#ifndef ADVENTURE2016_COMMANDHANDLE_HPP
#define ADVENTURE2016_COMMANDHANDLE_HPP

#include <string>
#include <vector>
#include <PlayerRole.hpp>
#include "Command.hpp"

class CommandHandle {
public:
    CommandHandle(const std::string& id, Command& command);

    Command& getCommand();
    const std::string& getId() const;
    const std::string& getDescription() const;
    const std::string& getUsage() const;
    const std::vector<std::string>& getInputBindings() const;
    PlayerRole getRole() const;

    void setDescription(const std::string& description);
    void setUsage(const std::string& usage);
    void addInputBinding(const std::string& inputBinding);
    void setRole(PlayerRole role);

private:
    Command& command;
    std::string id;
    std::string description;
    std::string usage;
    std::vector<std::string> inputBindings;
    PlayerRole role;
};


#endif //ADVENTURE2016_COMMANDHANDLE_HPP
