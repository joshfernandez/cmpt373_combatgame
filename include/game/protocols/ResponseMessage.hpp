#ifndef ADVENTURE2016_RESPONSEMESSAGE_HPP
#define ADVENTURE2016_RESPONSEMESSAGE_HPP

#include <string>
#include <yaml-cpp/yaml.h>

namespace protocols {

enum ResponseHeader {
    LOGIN_RESPONSE,
    REGISTER_RESPONSE,
    CHARACTER_LIST_RESPONSE,
    CHARACTER_LOAD_RESPONSE,
    DISPLAY_MESSAGE_RESPONSE,
    COMMAND_INFO_RESPONSE,
    COMBAT_INFO_RESPONSE,
    EDIT_INFO_RESPONSE,
};

struct ResponseMessage {
    ResponseHeader header;
    YAML::Node body;
};

std::string serializeResponseMessage(const ResponseMessage& request);

ResponseMessage deserializeResponseMessage(const std::string& serializedMessage);

}

#endif //ADVENTURE2016_RESPONSEMESSAGE_HPP
