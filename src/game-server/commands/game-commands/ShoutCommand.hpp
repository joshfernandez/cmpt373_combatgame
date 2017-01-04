#ifndef ADVENTURE2016_SHOUTCOMMAND_HPP
#define ADVENTURE2016_SHOUTCOMMAND_HPP

#include <commands/Command.hpp>
#include <game/GameModel.hpp>
#include <Controller.hpp>

class ShoutCommand : public Command{
public:
    ShoutCommand(GameModel& gameModel, Controller& controller);

    virtual std::unique_ptr<MessageBuilder> execute(const gsl::span<std::string, -1> arguments,
                                                    const PlayerInfo& player) override;

private:
    GameModel& gameModel;
    Controller& controller;
};


#endif //ADVENTURE2016_SHOUTCOMMAND_HPP
