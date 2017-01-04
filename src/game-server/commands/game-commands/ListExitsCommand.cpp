#include <commands/DisplayMessageBuilder.hpp>
#include "GameStrings.hpp"
#include "ListExitsCommand.hpp"

ListExitsCommand::ListExitsCommand(GameModel& gameModel) : gameModel{gameModel} {}

std::unique_ptr<MessageBuilder> ListExitsCommand::execute(const gsl::span<std::string, -1> arguments,
                                                          const PlayerInfo& player) {
    auto areaID = gameModel.getCharacterByID(player.playerID)->getAreaID();
    auto connectedAreaMap = *gameModel.getConnectedAreas(areaID);
    std::string message = GameStrings::get(GameStringKeys::EXITS_AREA) + "\n";
    for (auto mapEntry: connectedAreaMap) {
        message += ColorTag::GREEN + mapEntry.first + ColorTag::WHITE + "\n";
    }

    return DisplayMessageBuilder{message}
            .addClient(player.clientID)
            .setSender(DisplayMessageBuilder::SENDER_SERVER);
}
