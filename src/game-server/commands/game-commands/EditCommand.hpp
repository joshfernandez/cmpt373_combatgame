#ifndef ADVENTURE2016_EDITCOMMAND_HPP
#define ADVENTURE2016_EDITCOMMAND_HPP

#include <unordered_map>
#include <unordered_set>
#include <commands/Command.hpp>
#include "game/Area.hpp"

class GameModel;

class EditCommand : public Command {
public:
    EditCommand(GameModel& gameModel);
    virtual std::unique_ptr<MessageBuilder> execute(const gsl::span<std::string, -1> arguments,
                                                    const PlayerInfo& player) override;

private:
    bool playerIsEditing(const std::string& player);
    std::unique_ptr<MessageBuilder> addAreaToEdit(const Area& area, const PlayerInfo& player);
    std::unique_ptr<MessageBuilder> resume(const PlayerInfo& player);
    std::unique_ptr<MessageBuilder> editArea(const gsl::span<std::string, -1> arguments, const PlayerInfo& player);
    std::unique_ptr<MessageBuilder> saveChanges(const gsl::span<std::string, -1> arguments, const PlayerInfo& player);
    std::unique_ptr<MessageBuilder> cancel(const PlayerInfo& player);

    GameModel& gameModel;
    std::unordered_set<std::string> areas;
    std::unordered_map<std::string, std::string> playerToAreaIdMap;
};


#endif //ADVENTURE2016_EDITCOMMAND_HPP
