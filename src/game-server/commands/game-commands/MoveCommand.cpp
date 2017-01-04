#include <commands/DisplayMessageBuilder.hpp>
#include <boost/algorithm/string.hpp>
#include "GameStrings.hpp"
#include "MoveCommand.hpp"

MoveCommand::MoveCommand(GameModel& gameModel) : gameModel{gameModel} {}

std::unique_ptr<MessageBuilder> MoveCommand::execute(const gsl::span<std::string, -1> arguments,
                                                     const PlayerInfo& player) {
    std::string message;

    if (arguments.empty()) {
        message = GameStrings::get(GameStringKeys::UNSPECIFIED_EXIT);
    } else if (gameModel.moveCharacter(player.playerID, boost::algorithm::join(arguments, " "))) {
        auto areaID = gameModel.getCharacterByID(player.playerID)->getAreaID();
        auto area = gameModel.getAreaByID(areaID);
        message += "\n-- &g" + area->getTitle() + "&w --\n";
        message += area->getDescription();
    } else {
        message = GameStrings::get(GameStringKeys::INVALID_DIR);
    }

    return DisplayMessageBuilder{message}
            .addClient(player.clientID)
            .setSender(DisplayMessageBuilder::SENDER_SERVER);
}