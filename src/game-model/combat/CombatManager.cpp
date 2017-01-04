#include "CombatManager.hpp"

CombatManager::CombatManager() {
    setActionLookup(getDefaultActionMap());
}

void CombatManager::setAction(std::string actionID, std::string characterID) {

}

void CombatManager::setActionLookup(const std::unordered_map<std::string, std::shared_ptr<CombatAction>> actionLookup) {
    this->actionLookup = std::move(actionLookup);
}

void CombatManager::update() {
    for (CombatInstance& instance : combatInstances) {
        instance.update();

        if (instance.isBattleOver()) {
            instance.battleCleanup();
        }
    }

    auto eraseIter = std::remove_if(combatInstances.begin(), combatInstances.end(),
                   [] (const auto& ci) {
                       return ci.isBattleOver();
                   }
    );
    combatInstances.erase(eraseIter, combatInstances.end());
}

CombatInstance CombatManager::getNewCombatInstance() {
    return CombatInstance(&actionLookup, defaultActionID);
}

bool CombatManager::loadCombatInstance(CombatInstance& combatInstance) {
    bool bSetupSuccess = combatInstance.setupInstance();
    if (!bSetupSuccess) {
        return false;
    }
    combatInstances.push_back(std::move(combatInstance));
    return true;
}

CombatInstance* CombatManager::getCombatInstanceByCharacterID(const std::string& characterID) {
    for (auto& combatInstance : combatInstances) {
        auto characterInstance = combatInstance.getCharacterInstance(characterID);
        if (characterInstance != nullptr) {
            return &combatInstance;
        }
    }
    return nullptr;
}

CharacterInstance* CombatManager::getCharacterInstanceByCharacterID(const std::string& characterID) {
    for (auto& combatInstance : combatInstances) {
        auto characterInstance = combatInstance.getCharacterInstance(characterID);
        if (characterInstance != nullptr) {
            return characterInstance;
        }
    }
    return nullptr;
}

std::vector<std::string> CombatManager::getPossibleTargets(const std::string& characterID) {
    auto combatInstance = getCombatInstanceByCharacterID(characterID);
}

void CombatManager::addSpellAction(const Spell& spell) {
    if (spell.getType() == SpellType::BODY_SWAP) {

    } else {
        actionLookup.insert({ spell.getName(), std::make_shared<CombatCast>(spell) });
    }
}

std::unordered_map<std::string, std::shared_ptr<CombatAction>> CombatManager::getDefaultActionMap() {
    std::unordered_map<std::string, std::shared_ptr<CombatAction>> map;
    std::shared_ptr<CombatAttack> a = std::make_shared<CombatAttack>();
    map.insert({ a->getID(), a });
    return map;

}