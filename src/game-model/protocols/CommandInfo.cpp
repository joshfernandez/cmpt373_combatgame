#include <game/protocols/CommandInfo.hpp>
#include <glog/logging.h>

namespace {
const std::string CMD_TYPE_KEY = "type";
const std::string CMD_INFO_KEY = "body";

template <typename T>
T getEnumFromYaml(const YAML::Node& node, const std::string& key) {
    auto val = node[key];
    CHECK(val) << "Invalid node for data type";

    return static_cast<T>(val.as<int>());
}

}

namespace protocols {

RequestMessage createCommandInfoRequest(CommandName name) {
    YAML::Node node;
    node[CMD_TYPE_KEY] = static_cast<int>(name);

    return RequestMessage{header : RequestHeader::COMMAND_INFO_REQUEST, body : node};
}

CommandName readCommandInfoRequest(const RequestMessage& request) {
    CHECK(request.header == COMMAND_INFO_REQUEST) << "Invalid request type, expected command info request";

    return getEnumFromYaml<CommandName>(request.body, CMD_TYPE_KEY);
}

ResponseMessage createCommandInfoResponse(const CommandInfo& info) {
    YAML::Node node;
    node[CMD_TYPE_KEY] = static_cast<int>(info.name);
    node[CMD_INFO_KEY] = info.inputBinding;

    return ResponseMessage{header : ResponseHeader::COMMAND_INFO_RESPONSE, body : node};
}

CommandInfo readCommandInfoResponse(const ResponseMessage& responseMessage) {
    CHECK(responseMessage.header == ResponseHeader::COMMAND_INFO_RESPONSE) << "Invalid response type";

    auto name = getEnumFromYaml<CommandName>(responseMessage.body, CMD_TYPE_KEY);
    auto input = responseMessage.body[CMD_INFO_KEY].as<std::string>();
    return CommandInfo{name : name, inputBinding : input};
}

}