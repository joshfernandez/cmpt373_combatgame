#ifndef ADVENTURE2016_KICKCOMMAND_HPP
#define ADVENTURE2016_KICKCOMMAND_HPP

#include <commands/Command.hpp>

class Controller;

class KickCommand : public Command {
public:
    KickCommand(Controller& controller);

    virtual std::unique_ptr<MessageBuilder> execute(const gsl::span<std::string, -1> arguments,
                                                    const PlayerInfo& player) override;

private:
    Controller& controller;
};


#endif //ADVENTURE2016_KICKCOMMAND_HPP
