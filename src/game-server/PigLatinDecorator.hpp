#ifndef ADVENTURE2016_PIGLATINDECORATOR_HPP
#define ADVENTURE2016_PIGLATINDECORATOR_HPP

#include "commands/MessageBuilder.hpp"

class GameModel;
class Controller;

class PigLatinDecorator : public MessageBuilderDecorator {
public:
    PigLatinDecorator(const MessageBuilder& messageBuilder, const Controller& controller, GameModel& gameModel)
            : MessageBuilderDecorator{messageBuilder}, gameModel{gameModel}, controller{controller} {};
    virtual std::vector<MessageInfo> buildMessages() const override;

private:
    GameModel& gameModel;
    const Controller& controller;
};


#endif //ADVENTURE2016_PIGLATINDECORATOR_HPP
