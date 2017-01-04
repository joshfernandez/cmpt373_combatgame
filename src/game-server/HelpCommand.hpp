#ifndef ADVENTURE2016_HELPCOMMAND_HPP
#define ADVENTURE2016_HELPCOMMAND_HPP

#include <commands/Command.hpp>

class Controller;

class HelpCommand : public Command {
public:
    HelpCommand(Controller& controller) : controller{controller} {};
    ~HelpCommand() = default;

    virtual std::unique_ptr<MessageBuilder> execute(const gsl::span<std::string, -1> arguments,
                                                    const PlayerInfo& player) override;
private:
    Controller& controller;

    std::unique_ptr<MessageBuilder> allCommandsHelp(const networking::Connection& clientID);
    std::string getCommandBindingsHelpMessage(const std::string& command);
};


#endif //ADVENTURE2016_HELPCOMMAND_HPP
