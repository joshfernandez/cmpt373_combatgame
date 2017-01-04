#include "game/protocols/PlayerCommand.hpp"
#include "yaml-cpp/yaml.h"

namespace protocols
{

const std::string COMMAND_KEY = "cmd";
const std::string ARGUMENTS_KEY = "args";

RequestMessage createPlayerCommandRequestMessage(const PlayerCommand& playerCommand)
{
    YAML::Node data;
    data[COMMAND_KEY] = playerCommand.command;
    data[ARGUMENTS_KEY] = playerCommand.arguments;

    return RequestMessage{RequestHeader::PLAYER_COMMAND_REQUEST, data};
}

PlayerCommand readPlayerCommandRequestMessage(const RequestMessage& requestMessage)
{
    auto command = requestMessage.body[COMMAND_KEY].as<std::string>();
    auto arguments = requestMessage.body[ARGUMENTS_KEY].as<std::string>();

    return PlayerCommand{command, arguments};
}

}