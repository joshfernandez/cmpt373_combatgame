#ifndef ADVENTURE2016_LOOKCOMMAND_HPP
#define ADVENTURE2016_LOOKCOMMAND_HPP

#include <game/GameModel.hpp>
#include <commands/Command.hpp>

class LookCommand : public Command {
public:
    LookCommand(GameModel& gameModel);

    virtual std::unique_ptr<MessageBuilder> execute(const gsl::span<std::string, -1> arguments,
                                                    const PlayerInfo& player) override;

private:
    GameModel& gameModel;
};


#endif //ADVENTURE2016_LOOKCOMMAND_HPP
