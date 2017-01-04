#ifndef ADVENTURE2016_PLAYERCOMMANDS_HPP
#define ADVENTURE2016_PLAYERCOMMANDS_HPP

#include <string>
#include "RequestMessage.hpp"
#include "ResponseMessage.hpp"
#include "yaml-cpp/yaml.h"

namespace protocols {

struct PlayerCommand {
    std::string command;
    std::string arguments;
};

RequestMessage createPlayerCommandRequestMessage(const PlayerCommand& playerCommand);
PlayerCommand readPlayerCommandRequestMessage(const RequestMessage& requestMessage);
}

#endif //ADVENTURE2016_PLAYERCOMMANDS_HPP
