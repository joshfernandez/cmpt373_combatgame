#ifndef ADVENTURE2016_CASTCOMMAND_H
#define ADVENTURE2016_CASTCOMMAND_H

#include <commands/Command.hpp>
#include <game/GameModel.hpp>
#include <Controller.hpp>

class CastCommand : public Command {
public:
    CastCommand(GameModel& gameModel, Controller& controller);
    virtual std::unique_ptr<MessageBuilder> execute(const gsl::span<std::string, -1> arguments,
                                                    const PlayerInfo& player) override;

private:
    GameModel& gameModel;
	Controller& controller;

};


#endif //ADVENTURE2016_CASTCOMMAND_H
