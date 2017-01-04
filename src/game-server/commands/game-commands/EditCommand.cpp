#include <glog/logging.h>
#include <boost/algorithm/string.hpp>
#include "EditCommand.hpp"
#include "EditMessageBuilder.hpp"
#include "game/GameModel.hpp"
#include "game/GameDataImporter.hpp"

namespace {
enum class ArgToken {
    AREA,
    SUBMIT,
    RESUME,
    CANCEL,
};

const std::string AREA_TOKEN = "area";
const std::string COMMIT_TOKEN = "submit";
const std::string RESUME_TOKEN = "resume";
const std::string CANCEL_TOKEN = "cancel";

std::unordered_map<std::string, ArgToken> tokenMap{
        {AREA_TOKEN,   ArgToken::AREA},
        {COMMIT_TOKEN, ArgToken::SUBMIT},
        {RESUME_TOKEN, ArgToken::RESUME},
        {CANCEL_TOKEN, ArgToken::CANCEL}
};

std::unique_ptr<MessageBuilder> areaInUse(const networking::Connection& player) {
    return DisplayMessageBuilder{GameStrings::get(GameStringKeys::EDIT_IN_USE)}
            .addClient(player)
            .setSender(GameStrings::get(GameStringKeys::SERVER_NAME));
}

std::unique_ptr<MessageBuilder> cancelMsg(const networking::Connection& player) {
    return DisplayMessageBuilder{GameStrings::get(GameStringKeys::EDIT_CANCEL)}
            .addClient(player)
            .setSender(GameStrings::get(GameStringKeys::SERVER_NAME));
}

std::unique_ptr<MessageBuilder> alreadyEditing(const networking::Connection& player) {
    return DisplayMessageBuilder{GameStrings::get(GameStringKeys::EDIT_ALREADY_EDITING)}
            .addClient(player)
            .setSender(GameStrings::get(GameStringKeys::SERVER_NAME));
}

std::unique_ptr<MessageBuilder> submitError(std::string error, const networking::Connection& player) {
    protocols::EditResponse editResponse;
    editResponse.success = false;
    editResponse.editType = protocols::EditType::SAVE_CHANGES;
    editResponse.message = std::move(error);
    return std::make_unique<EditMessageBuilder>(player, editResponse);
}

std::unique_ptr<MessageBuilder> submitSuccess(std::string error, const networking::Connection& player) {
    protocols::EditResponse editResponse;
    editResponse.success = true;
    editResponse.editType = protocols::EditType::SAVE_CHANGES;
    editResponse.message = std::move(error);
    return std::make_unique<EditMessageBuilder>(player, editResponse);
}

std::unique_ptr<MessageBuilder> notEditing(const networking::Connection& player) {
    return DisplayMessageBuilder{GameStrings::get(GameStringKeys::EDIT_NOT_EDITING)}.addClient(player);
}

}

EditCommand::EditCommand(GameModel& gameModel) : gameModel{gameModel} {}

std::unique_ptr<MessageBuilder> EditCommand::execute(const gsl::span<std::string, -1> arguments,
                                                     const PlayerInfo& player) {
    if (arguments.length() == 0) {
        return DisplayMessageBuilder{GameStrings::get(GameStringKeys::EDIT_AREA_HELP)}
                .addClient(player.clientID)
                .setSender(GameStrings::get(GameStringKeys::SERVER_NAME));
    }

    auto argument = tokenMap.find(arguments[0]);
    if (argument == tokenMap.end()) {
        return DisplayMessageBuilder{GameStrings::get(GameStringKeys::EDIT_INVALID_ARG)}
                .addClient(player.clientID)
                .setSender(GameStrings::get(GameStringKeys::SERVER_NAME));
    }

    switch (argument->second) {
        case ArgToken::AREA:
            return editArea(arguments, player);
        case ArgToken::SUBMIT:
            return saveChanges(arguments, player);
        case ArgToken::RESUME:
            return resume(player);
        case ArgToken::CANCEL:
            return cancel(player);
    }
}

std::unique_ptr<MessageBuilder> EditCommand::editArea(const gsl::span<std::string, -1> arguments,
                                                      const PlayerInfo& player) {
    //arguments: area [areaid]
    CHECK(!arguments.empty()) << "Arguments should not be empty";

    Area* area;
    //Didnt specify area id
    if (arguments.size() == 1) {
        auto character = gameModel.getCharacterByID(player.playerID);
        area = gameModel.getAreaByID(character->getAreaID());
    } else {
        area = gameModel.getAreaByID(arguments[1]);
        if (area == nullptr) {
            auto newArea = Area{arguments[1]};
            newArea.setTitle("Area" + arguments[1]);
            newArea.setDescription("Description");
            area = &newArea;
        }
    }

    return addAreaToEdit(*area, player);
}

std::unique_ptr<MessageBuilder> EditCommand::addAreaToEdit(const Area& area, const PlayerInfo& player) {
    protocols::EditResponse editResponse;
    editResponse.success = true;
    editResponse.editType = protocols::EditType::AREA;

    if (playerIsEditing(player.playerID)) {
        return alreadyEditing(player.clientID);
    }

    if (areas.count(area.getID()) == 1) {
        return areaInUse(player.clientID);
    }

    areas.emplace(area.getID());
    playerToAreaIdMap.emplace(player.playerID, area.getID());

    editResponse.data = area;
    return std::make_unique<EditMessageBuilder>(player.clientID, editResponse);
}

bool EditCommand::playerIsEditing(const std::string& player) {
    return playerToAreaIdMap.count(player) == 1;
}

std::unique_ptr<MessageBuilder> EditCommand::saveChanges(const gsl::span<std::string, -1> arguments,
                                                         const PlayerInfo& player) {
    if (!playerIsEditing(player.playerID)) {
        return notEditing(player.clientID);
    }
    try {
        auto ymlStr = arguments[1];
        LOG(INFO) << ymlStr;
        auto area = YAML::Load(ymlStr).as<Area>();

        gameModel.addArea(area);

        playerToAreaIdMap.erase(player.playerID);
        areas.erase(area.getID());

        return submitSuccess(GameStrings::get(GameStringKeys::EDIT_SAVED), player.clientID);
    } catch (std::exception& e) {
        LOG(INFO) << "Error when user submitted area: " << e.what();
        YAML::Emitter em;
        em << arguments[1];
        LOG(INFO) << em.c_str();
        return submitError(e.what(), player.clientID);
    }
}

std::unique_ptr<MessageBuilder> EditCommand::resume(const PlayerInfo& player) {
    if (!playerIsEditing(player.playerID)) {
        return notEditing(player.clientID);
    }

    protocols::EditResponse editResponse;
    editResponse.success = true;
    editResponse.editType = protocols::EditType::AREA;
    editResponse.data = *gameModel.getAreaByID(playerToAreaIdMap.at(player.playerID));

    return std::make_unique<EditMessageBuilder>(player.clientID, editResponse);
}

std::unique_ptr<MessageBuilder> EditCommand::cancel(const PlayerInfo& player) {
    if (!playerIsEditing(player.playerID)) {
        return notEditing(player.clientID);
    }

    auto area = playerToAreaIdMap.at(player.playerID);
    playerToAreaIdMap.erase(player.playerID);
    areas.erase(area);

    return cancelMsg(player.clientID);
}
