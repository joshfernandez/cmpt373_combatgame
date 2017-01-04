#ifndef ADVENTURE2016_SAYCOMMAND_HPP
#define ADVENTURE2016_SAYCOMMAND_HPP

#include <commands/Command.hpp>
#include <Controller.hpp>
#include <game/GameModel.hpp>

#include <gsl/gsl>

class SayCommand : public Command {
public:
    SayCommand(GameModel& gameModel, Controller& controller);

    virtual std::unique_ptr<MessageBuilder> execute(const gsl::span<std::string, -1> arguments,
                                                    const PlayerInfo& player) override;

private:
	GameModel& gameModel;
    Controller& controller;
};


#endif //ADVENTURE2016_SAYCOMMAND_HPP
