
#include <commands/DisplayMessageBuilder.hpp>
#include <boost/algorithm/string/join.hpp>
#include "CastCommand.hpp"

CastCommand::CastCommand(GameModel &gameModel, Controller& controller) : gameModel{gameModel}, controller{controller} {}

std::unique_ptr<MessageBuilder> CastCommand::execute(const gsl::span<std::string, -1> arguments, const PlayerInfo &player) {
	if (arguments.empty()) {
		gameModel.listValidSpells(player.playerID);
		return DisplayMessageBuilder{GameStrings::get(GameStringKeys::SPELL_LIST)};
	}
	else if (arguments.size() == 1) {
		gameModel.getCharacterByID(player.playerID)->pushToBuffer(GameStrings::get(GameStringKeys::SPELL_INVALID_USAGE),
																  GameStringKeys::MESSAGE_SENDER_SERVER, ColorTag::WHITE);
		return DisplayMessageBuilder{GameStrings::get(GameStringKeys::SPELL_INVALID_USAGE)};
	}

	auto spellStrings = arguments.subspan(0, arguments.size() - 1);
	std::string spell = boost::algorithm::join(spellStrings, " ");
	std::string targetID = *(arguments.end() - 1);

	if (!controller.getClientID(targetID)) {
		if (targetID == TARGET_SELF) {
			targetID = player.playerID;
		}
		else {
			auto targetNPC = gameModel.getNPCInArea(targetID, gameModel.getCharacterByID(player.playerID)->getAreaID());
			if (targetNPC == nullptr) {
				std::string invalidPlayer = targetID + " " + GameStrings::get(GameStringKeys::INVALID_TGT);
				gameModel.getCharacterByID(player.playerID)->pushToBuffer(invalidPlayer, GameStringKeys::MESSAGE_SENDER_SERVER, ColorTag::WHITE);
				return DisplayMessageBuilder{invalidPlayer};
			}
			targetID = targetNPC->getID();
		}
	}

	gameModel.castSpell(player.playerID, targetID, spell);
	return buildPlayerMessage(player.clientID, "Casting spell " + ColorTag::CYAN + spell + ColorTag::WHITE);

}
