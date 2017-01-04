#ifndef ADVENTURE2016_MESSAGE_H
#define ADVENTURE2016_MESSAGE_H

#include <string>
#include <yaml-cpp/yaml.h>

namespace protocols {

enum RequestHeader {
    LOGIN_REQUEST,
    REGISTER_REQUEST,
    CHARACTER_LIST_REQUEST,
    CHARACTER_LOAD_REQUEST,
    PLAYER_COMMAND_REQUEST,
    COMMAND_INFO_REQUEST,
};

struct RequestMessage {
    RequestHeader header;
    YAML::Node body;
};

std::string serializeRequestMessage(const RequestMessage& request);

RequestMessage deserializeRequestMessage(const std::string& serializedMessage);

}

#endif