#include <glog/logging.h>
#include "CommandInfoRequestHandler.hpp"
#include "CommandConfig.hpp"
#include "MessageIO.hpp"
#include "CommandList.hpp"

CommandInfoRequestHandler::CommandInfoRequestHandler(MessageIO& messageIO, CommandConfig& commandConfig)
        : io{messageIO},
          commandConfig{commandConfig} {}

void CommandInfoRequestHandler::handleDataRequest(protocols::CommandName name,
                                                  const networking::Connection& requester) {
    std::string commandId;
    switch (name) {
        case protocols::CommandName::EDIT:
            commandId = COMMAND_EDIT;
            break;
        case protocols::CommandName::COMBAT:
            commandId = COMMAND_ATTACK;
            break;
        case protocols::CommandName::CAST:
            commandId = COMMAND_CAST;
            break;
        default:
            LOG(ERROR) << "Case not handled";
    }

    auto inputs = commandConfig.getCommandInputBindings(commandId);
    auto cmdInfo = protocols::CommandInfo{name : name, inputBinding : inputs[0]};
    io.send(cmdInfo, requester);
}