#ifndef ADVENTURE2016_ATTACKCOMMAND_H
#define ADVENTURE2016_ATTACKCOMMAND_H

#include <commands/DisplayMessageBuilder.hpp>
#include <commands/Command.hpp>
#include <game/GameModel.hpp>
#include <Controller.hpp>

class AttackCommand : public Command {
public:
    AttackCommand(GameModel &gameModel, Controller &controller);
    virtual std::unique_ptr<MessageBuilder> execute(const gsl::span<std::string, -1> arguments,
                                                    const PlayerInfo& player) override;

private:
    GameModel& gameModel;
    Controller& controller;

    std::string NOT_FOUND = "Not found";
    std::string PLAYER_IN_BATTLE = "You are already in battle";
    std::string TARGET_IN_BATTLE = "Your target is already in battle";
    std::string NOT_IN_COMBAT = "Attack what?";
    std::string SET_COMBAT_ACTION = "You start attacking your opponent";
    std::string COMBAT_ENGAGED = "You engage in combat with: ";

    std::unique_ptr<MessageBuilder> setCombatAction(const PlayerInfo &info);
};


#endif //ADVENTURE2016_ATTACKCOMMAND_H
