#ifndef ADVENTURE2016_GAMEFUNCTIONS_H
#define ADVENTURE2016_GAMEFUNCTIONS_H

#include "Controller.hpp"
#include "commands/game-commands/GameCommands.hpp"
#include "commands/DisplayMessageBuilder.hpp"

class GameFunctions {
public:
    GameFunctions(Controller& controller);

private:
    void registerCommands();
    Controller& controller;
    GameModel& gameModel;

    ListExitsCommand listExitsCommand;
    ListPlayersCommand listPlayersCommand;
    LookCommand lookCommand;
    MoveCommand moveCommand;
    SayCommand sayCommand;
    ShoutCommand shoutCommand;
    WhisperCommand whisperCommand;
    AttackCommand attackCommand;
    CastCommand castCommand;
	StatusCommand statusCommand;
	KickCommand kickCommand;
	EditCommand editCommand;
};


#endif //ADVENTURE2016_GAMEFUNCTIONS_H
