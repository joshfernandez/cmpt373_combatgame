#ifndef ADVENTURE2016_COMMANDINFO_HPP
#define ADVENTURE2016_COMMANDINFO_HPP

#include "RequestMessage.hpp"
#include "ResponseMessage.hpp"

namespace protocols {

enum class CommandName {
    EDIT,
    COMBAT,
    CAST,
};

struct CommandInfo {
    CommandName name;
    std::string inputBinding;
};

RequestMessage createCommandInfoRequest(CommandName name);
CommandName readCommandInfoRequest(const RequestMessage& request);

ResponseMessage createCommandInfoResponse(const CommandInfo& info);
CommandInfo readCommandInfoResponse(const ResponseMessage& responseMessage);

}

#endif //ADVENTURE2016_COMMANDINFO_HPP
