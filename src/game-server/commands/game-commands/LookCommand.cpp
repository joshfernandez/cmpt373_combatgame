#include <commands/DisplayMessageBuilder.hpp>
#include <boost/algorithm/string/join.hpp>
#include "LookCommand.hpp"

LookCommand::LookCommand(GameModel& gameModel) : gameModel{gameModel} {}

std::unique_ptr<MessageBuilder> LookCommand::execute(const gsl::span<std::string, -1> arguments,
													 const PlayerInfo& player) {
	auto areaID = gameModel.getCharacterByID(player.playerID)->getAreaID();
	std::string description;

	if (arguments.empty()) {
		description = "\n-- &g" + gameModel.getAreaByID(areaID)->getTitle() + "&w --\n" + gameModel.getAreaDescription(areaID);
		gameModel.getCharacterByID(player.playerID)->pushToBuffer(description, GameStringKeys::MESSAGE_SENDER_SERVER,
																  ColorTag::WHITE);
		return DisplayMessageBuilder{description};
	}

	auto targetEntity = arguments[0];
	auto npc = gameModel.getNPCInArea(targetEntity, areaID);
	auto object = gameModel.getObjectInArea(targetEntity, areaID);
	auto extDesc = gameModel.getExtendedDescription(targetEntity, areaID);
	if (npc != nullptr) {
		description = npc->getlongDesc();
	}
	else if (object != nullptr) {
		description = object->getDescription();
	}
	else if (extDesc) {
		description = extDesc.get();
	}
	else {
		description = targetEntity + " " + GameStrings::get(GameStringKeys::INVALID_TGT);
	}


	gameModel.getCharacterByID(player.playerID)->pushToBuffer(description, GameStringKeys::MESSAGE_SENDER_SERVER, ColorTag::WHITE);
	return DisplayMessageBuilder{description};
}
