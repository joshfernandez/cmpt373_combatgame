#include "GameFunctions.hpp"
#include "commands/CommandList.hpp"


GameFunctions::GameFunctions(Controller& controller)
        : controller{controller}, gameModel{controller.getGameModel()},
          listExitsCommand{gameModel},
          listPlayersCommand{gameModel},
          lookCommand{gameModel},
          moveCommand{gameModel},
          sayCommand{gameModel, controller},
          shoutCommand{gameModel, controller},
          whisperCommand{gameModel, controller},
          attackCommand{gameModel, controller},
          castCommand{gameModel, controller},
          statusCommand{gameModel},
          kickCommand{controller},
          editCommand{gameModel} {
    registerCommands();
}

void GameFunctions::registerCommands() {
    controller.registerCommand(COMMAND_EXITS, listExitsCommand);
    controller.registerCommand(COMMAND_PLAYERS, listPlayersCommand);
    controller.registerCommand(COMMAND_LOOK, lookCommand);
    controller.registerCommand(COMMAND_MOVE, moveCommand);
    controller.registerCommand(COMMAND_SAY, sayCommand);
    controller.registerCommand(COMMAND_SHOUT, shoutCommand);
    controller.registerCommand(COMMAND_WHISPER, whisperCommand);
    controller.registerCommand(COMMAND_ATTACK, attackCommand);
    controller.registerCommand(COMMAND_CAST, castCommand);
    controller.registerCommand(COMMAND_STATUS, statusCommand);
    controller.registerCommand(COMMAND_KICK, kickCommand);
    controller.registerCommand(COMMAND_EDIT, editCommand);
}



