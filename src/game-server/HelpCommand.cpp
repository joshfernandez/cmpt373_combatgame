#include <boost/algorithm/string/join.hpp>
#include <GameStrings.hpp>
#include "HelpCommand.hpp"
#include "Controller.hpp"

std::unique_ptr<MessageBuilder> HelpCommand::execute(const gsl::span<std::string, -1> arguments,
                                                                 const PlayerInfo& player) {
    if (arguments.empty()) {
        return allCommandsHelp(player.clientID);
    }

    auto accountInfo = controller.getAccountInfo(player.clientID);
    auto command = arguments[0];
    auto it = controller.inputToCommandMap.find(command);
    if (it == controller.inputToCommandMap.end() || !accountInfo.hasRole(it->second.getRole())) {
        auto message = "Command <" + command + "> not found.";
        return DisplayMessageBuilder{message}.addClient(player.clientID)
                .setSender(DisplayMessageBuilder::SENDER_SERVER);
    }

    std::string message;
    message = "Help for command <" + command + ">\n";
    message += "\tDescription: " + it->second.getDescription() + "\n";
    message += "\tUsage: " + command + " " + it->second.getUsage() + "\n";
    message += "\t" + getCommandBindingsHelpMessage(command);

    return DisplayMessageBuilder{message}.addClient(player.clientID)
            .setSender(DisplayMessageBuilder::SENDER_SERVER);
}

std::unique_ptr<MessageBuilder> HelpCommand::allCommandsHelp(const networking::Connection& clientID) {
    auto accountInfo = controller.getAccountInfo(clientID);

    //Get all unique commands
    std::unordered_set<std::string> commands;
    for (const auto& it : controller.inputToCommandMap) {
        if (!accountInfo.hasRole(it.second.getRole())) {
            continue;
        }

        commands.insert(it.second.getId());
    }

    std::string message = "List of available commands. Type help <command> for more information\n";
    for (const auto& cmd : commands) {
        message += "Name: " + cmd;
        message += " - " + getCommandBindingsHelpMessage(cmd);
        message += "\n";
    }

    return DisplayMessageBuilder{message}.addClient(clientID)
            .setSender(DisplayMessageBuilder::SENDER_SERVER);
}

std::string HelpCommand::getCommandBindingsHelpMessage(const std::string& command) {
    std::string message;
    message += "Command:[" + ColorTag::CYAN;
    message += boost::algorithm::join(controller.inputToCommandMap.at(command).getInputBindings(), "&w,&g");
    message += ColorTag::WHITE + "]";

    return message;
}