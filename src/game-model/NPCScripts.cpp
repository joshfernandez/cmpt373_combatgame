#include <game/NPCScripts.hpp>

NPCScripts::NPCScripts(const std::string &scriptingQualifier,
                       const std::vector<std::string> &scriptingCommands,
                       const std::string &scriptingName,
                       const std::string &scriptingDescription
                      )

                      :
                       scriptingQualifier(scriptingQualifier)
                      , scriptingCommands(scriptingCommands)
                      , scriptingName(scriptingName)
                      , scriptingDescription(scriptingDescription)

                      {

                      }

const std::vector<std::string> NPCScripts::defaultCommand = {" ", " "};
const std::string NPCScripts::defaultQualifier = " ";
const std::string NPCScripts::defaultScriptingName = {" ", " "};
const std::vector<std::string> NPCScripts::defaultScriptingDescription = {" ", " "};

std::string NPCScripts::getScriptingQualifier() const {
  return scriptingQualifier;
};

std::vector<std::string> NPCScripts::getScriptingCommands() const {
    return scriptingCommands;
}

std::string NPCScripts::getScriptingName() const {
    return scriptingName;
};

std::string NPCScripts::getScriptingDescription() const {
    return scriptingDescription;
}