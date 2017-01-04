#ifndef ADVENTURE2016_MOVECOMMAND_HPP
#define ADVENTURE2016_MOVECOMMAND_HPP

#include <commands/Command.hpp>
#include <game/GameModel.hpp>

class MoveCommand : public Command {
public:
    MoveCommand(GameModel& gameModel);

    virtual std::unique_ptr<MessageBuilder> execute(const gsl::span<std::string, -1> arguments,
                                                    const PlayerInfo& player) override;

private:
    GameModel& gameModel;
};


#endif //ADVENTURE2016_MOVECOMMAND_HPP
