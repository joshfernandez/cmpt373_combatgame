#include "GameStrings.hpp"
#include <Controller.hpp>
#include "KickCommand.hpp"

KickCommand::KickCommand(Controller& controller) : controller{controller} {}

std::unique_ptr<MessageBuilder> KickCommand::execute(const gsl::span<std::string, -1> arguments,
                                                     const PlayerInfo& player) {
    if (arguments.empty()) {
        return DisplayMessageBuilder{GameStrings::get(GameStringKeys::UNSPECIFIED_PLAYER)}
                .addClient(player.clientID)
                .setSender(GameStrings::get(GameStringKeys::SERVER_NAME));
    }

    auto targetPlayer = arguments[0];
    auto targetClient = controller.getClientID(targetPlayer);
    if (!targetClient) {
        return DisplayMessageBuilder{targetPlayer + " " + GameStrings::get(GameStringKeys::INVALID_TGT)}
                .addClient(player.clientID)
                .setSender(GameStrings::get(GameStringKeys::SERVER_NAME));
    }

    controller.disconnectPlayer(targetPlayer);
    auto clients = controller.getAllClients();
    return DisplayMessageBuilder{"<" + ColorTag::MAGENTA +  targetPlayer + "> "
                                 + ColorTag::WHITE + GameStrings::get(GameStringKeys::PLAYER_KICKED)}
            .addClients(clients)
            .setSender(GameStrings::get(GameStringKeys::SERVER_NAME));
}