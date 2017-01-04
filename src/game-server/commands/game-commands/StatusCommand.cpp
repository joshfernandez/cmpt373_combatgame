
#include "StatusCommand.hpp"

StatusCommand::StatusCommand(GameModel& gameModel) : gameModel{gameModel} {}

std::unique_ptr<MessageBuilder> StatusCommand::execute(const gsl::span<std::string, -1> arguments,
													 const PlayerInfo& player) {
	std::string status = gameModel.getCharacterByID(player.playerID)->getStatus();
	return buildPlayerMessage(player.clientID, status);
}