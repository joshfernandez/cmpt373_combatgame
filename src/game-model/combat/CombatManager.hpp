#ifndef COMBATMANAGER_HPP
#define COMBATMANAGER_HPP

#include <string>
#include "CombatInstance.hpp"

class CombatManager {

public:
    CombatManager();
    void update();
    void addSpellAction(const Spell& spell);
    void setActionLookup(const std::unordered_map<std::string, std::shared_ptr<CombatAction>> actionLookup);
    void setAction(std::string actionID, std::string characterID);
    CombatInstance getNewCombatInstance();

    bool loadCombatInstance(CombatInstance& combatInstance);
    CombatInstance* getCombatInstanceByCharacterID(const std::string& characterID);
    CharacterInstance* getCharacterInstanceByCharacterID(const std::string& characterID);
    std::vector<std::string> getPossibleTargets(const std::string& characterID);

private:

    // TODO: Make this configurable
    std::string defaultActionID = "attack";
    std::unordered_map<std::string, std::shared_ptr<CombatAction>> actionLookup;
    std::vector<CombatInstance> combatInstances;

    static std::unordered_map<std::string, std::shared_ptr<CombatAction>> getDefaultActionMap();
};

#endif