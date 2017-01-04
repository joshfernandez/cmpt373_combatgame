#include <commands/DisplayMessageBuilder.hpp>
#include "WhisperCommand.hpp"
#include <boost/algorithm/string.hpp>
#include "GameStrings.hpp"

WhisperCommand::WhisperCommand(GameModel& gameModel, Controller& controller) : gameModel{gameModel}, controller{controller} {}

std::unique_ptr<MessageBuilder> WhisperCommand::execute(const gsl::span<std::string, -1> arguments,
                                                        const PlayerInfo& player) {
    if (arguments.empty()) {
		gameModel.getCharacterByID(player.playerID)->pushToBuffer(GameStrings::get(GameStringKeys::UNSPECIFIED_PLAYER),
																  GameStringKeys::MESSAGE_SENDER_SERVER, ColorTag::WHITE);
		return DisplayMessageBuilder{GameStrings::get(GameStringKeys::UNSPECIFIED_PLAYER)};
    }

    auto targetPlayerID = arguments[0];
    auto targetClient = controller.getClientID(targetPlayerID);
	auto message = boost::algorithm::join(arguments.subspan(1, arguments.length() - 1), " ");

    if (!targetClient) {
		gameModel.getCharacterByID(player.playerID)->pushToBuffer(GameStrings::get(GameStringKeys::PLAYER)
															  + " " + targetPlayerID + " "
															  + GameStrings::get(GameStringKeys::INVALID_TGT),
																  GameStringKeys::MESSAGE_SENDER_SERVER, ColorTag::WHITE);
		return DisplayMessageBuilder{message};
    }

	gameModel.sendPrivateMessage(player.playerID, message, targetPlayerID);
    return DisplayMessageBuilder{message};
}
