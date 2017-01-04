#include "AttackCommand.hpp"
#include <iostream>
#include <boost/algorithm/string/join.hpp>

AttackCommand::AttackCommand(GameModel &gameModel, Controller &controller)
        : gameModel{gameModel}, controller{controller} {
}

std::unique_ptr<MessageBuilder>
AttackCommand::execute(const gsl::span<std::string, -1> arguments, const PlayerInfo &player) {
    if (arguments.empty()) {
        return setCombatAction(player);
    }
	std::string targetID = boost::algorithm::join(arguments, " ");
    auto targetClient = controller.getClientID(targetID);

    if (!targetClient) {
		auto targetNPC = gameModel.getNPCInArea(targetID, gameModel.getCharacterByID(player.playerID)->getAreaID());
		if (targetNPC == nullptr) {
			std::string invalidPlayer = targetID + " " + NOT_FOUND;
			return buildPlayerMessage(player.clientID, invalidPlayer);
		}
        targetID = targetNPC->getID();
    }

    if (gameModel.characterIsInCombat(player.playerID)) {
        return buildPlayerMessage(player.clientID, PLAYER_IN_BATTLE);
    }

    if (gameModel.characterIsInCombat(targetID)) {
        return buildPlayerMessage(player.clientID, TARGET_IN_BATTLE);
    }

    if (!gameModel.engageCharacterInCombat(player.playerID, targetID)) {
        std::cerr << "Invalid state in AttackCommand";
    }
    return DisplayMessageBuilder{COMBAT_ENGAGED + " " + gameModel.getCharacterByID(targetID)->getName()}
            .addClient(player.clientID)
            .setSender(DisplayMessageBuilder::SENDER_SERVER);
}


std::unique_ptr<MessageBuilder>
AttackCommand::setCombatAction(const PlayerInfo &player) {
    if (gameModel.getCharacterByID(player.playerID)->getState() != CharacterState::BATTLE) {
        return buildPlayerMessage(player.clientID, NOT_IN_COMBAT);
    }

    gameModel.setCombatAction(player.playerID, "attack");
    return buildPlayerMessage(player.clientID, SET_COMBAT_ACTION);
}

