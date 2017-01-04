#ifndef ADVENTURE2016_NPCSCRIPTS_HPP
#define ADVENTURE2016_NPCSCRIPTS_HPP

//#include <game/Character.hpp>
#include <vector>
#include <string>
#include <unordered_map>

class NPCScripts {
public:

    NPCScripts(     const std::string& scriptingQualifier,
                    const std::vector<std::string>& scriptingCommands,
                    const std::string& scriptingName,
                    const std::string& scriptingDescription
    );

    std::string getScriptingQualifier() const;
    std::vector<std::string> getScriptingCommands() const;
    std::string getScriptingName() const;
    std::string getScriptingDescription() const;

    //Defaults
    static const std::vector<std::string> defaultCommand;
    static const std::string defaultQualifier;
    static const std::string defaultScriptingName;
    static const std::vector<std::string> defaultScriptingDescription;

private:
    std::string scriptingQualifier;
    std::vector<std::string> scriptingCommands;
    std::string scriptingName;
    std::string scriptingDescription;
};

#endif //ADVENTURE2016_NPCSCRIPTS_HPP
