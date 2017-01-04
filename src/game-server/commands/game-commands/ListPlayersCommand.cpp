#include <commands/DisplayMessageBuilder.hpp>
#include <boost/algorithm/string.hpp>
#include "GameStrings.hpp"
#include "ListPlayersCommand.hpp"

ListPlayersCommand::ListPlayersCommand(GameModel& gameModel) : gameModel{gameModel} {}

std::unique_ptr<MessageBuilder> ListPlayersCommand::execute(const gsl::span<std::string, -1> arguments,
                                                            const PlayerInfo& player) {
    auto areaID = gameModel.getCharacterByID(player.playerID)->getAreaID();
    std::string message = GameStrings::get(GameStringKeys::PLAYERS_AREA) + "\n";
    for (const auto& playerID: gameModel.getPlayerIDsInArea(areaID)) {
        message += gameModel.getCharacterByID(playerID)->getName() + " \n";
    }
	for (const auto& NPCID: gameModel.getNPCIDsInArea(areaID)) {
		message += gameModel.getNPCByID(NPCID)->getName() + " \n";
	}

    return DisplayMessageBuilder{message}
            .addClient(player.clientID)
            .setSender(DisplayMessageBuilder::SENDER_SERVER);
}


