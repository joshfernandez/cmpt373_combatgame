#include <commands/DisplayMessageBuilder.hpp>
#include "GameStrings.hpp"
#include "ShoutCommand.hpp"
#include <boost/algorithm/string.hpp>

ShoutCommand::ShoutCommand(GameModel& gameModel, Controller& controller) : gameModel{gameModel}, controller{controller} {}

std::unique_ptr<MessageBuilder> ShoutCommand::execute(const gsl::span<std::string, -1> arguments,
                                                      const PlayerInfo& player) {
    auto message = boost::algorithm::join(arguments, " ");
    gameModel.sendLocalMessageFromCharacter(player.playerID, message);
    return DisplayMessageBuilder{message};
}
