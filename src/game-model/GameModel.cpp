#include <boost/format.hpp>
#include "game/GameModel.hpp"

GameModel::GameModel() {
    // TODO: Move this to something more elegant
    loadDefaultSpells();
}

/*
 *	CHARACTER FUNCTIONS
 */

const std::string ACTION_NPC = "npc";
const std::string ACTION_OBJECT = "object";

bool GameModel::createCharacter(const std::string& characterID) {

    Character character = YmlSerializer::load_from_file(characterID);
    if (character.getAreaID() == "-1") {
        character.setAreaID(this->defaultLocation);
    }
	characters.insert(std::pair<std::string, Character>(characterID, character));

	return true;
}

Object* GameModel::getObjectById(const std::string& objectID) {
	if (objects.find(objectID) != objects.end()) {
		return (Object*)&(objects.at(objectID));
	}
	return nullptr;
}

Object* GameModel::getObjectInArea(const std::string& keyword, const std::string& areaID) {
	auto area = getAreaByID(areaID);
	for (auto& objectID : area->getObjectNames()) {
		auto object = getObjectById(objectID);
		for (auto& objectKeyword: object->getKeywords()) {
			if (keyword == objectKeyword) {
				return object;
			}
		}
	}
	return nullptr;
}
/*
 *	AREA FUNCTIONS
 */

bool GameModel::addArea(const Area area) {
    locations.erase(area.getID());
	locations.emplace(area.getID(), std::move(area));

	// No failure case yet...
	return true;
}

Area* GameModel::getAreaByID(const std::string& areaID) const {

    if (locations.count(areaID) == 1) {

        return (Area*)&(locations.at(areaID));
    }

	return nullptr;
}

std::string GameModel::getAreaDescription(const std::string& areaID) const {
	auto area = getAreaByID(areaID);
	if (area == nullptr) {
		return "Area does not exist.\n";
	}
	return area->getDescription();
}

boost::optional<std::string> GameModel::getExtendedDescription(const std::string& keyword, const std::string& areaID) {
	auto descriptionsMap = getAreaByID(areaID)->getExtendedDescriptions();
	if (descriptionsMap.find(keyword) != descriptionsMap.end()) {
		return descriptionsMap.at(keyword);
	}
	return boost::optional<std::string>{};
}

std::unordered_map<std::string, std::string>* GameModel::getConnectedAreas(const std::string& areaID) const {
    auto area = getAreaByID(areaID);
    return area->getConnectedAreas();
}

bool GameModel::moveCharacter(const std::string& characterID, const std::string& areaTag) {

    auto character = getCharacterByID(characterID);
    if (character == nullptr) {
        return false;
    }

    if (!characterCanMove(*character)) {
        return false;
    }

    auto currAreaID = character->getAreaID();
    auto area = getAreaByID(currAreaID);
    if (area == nullptr) {
        return false;
    }

    auto connectedAreas = area->getConnectedAreas();

    // Check if the current area is connected to the target destination
    if (connectedAreas->count(areaTag) == 0) {
        return false;
    }
    auto connectedArea = connectedAreas->at(areaTag);

    character->setAreaID(connectedArea);
    auto newDir = getRelativeDirection(findDirectionByAreaID(connectedArea, currAreaID));
    sendMoveUpdateMessages(character->getID(), currAreaID, getRelativeDirection(areaTag), character->getAreaID(), newDir);

    return true;

}

std::string GameModel::findDirectionByAreaID(const std::string& sourceID, const std::string& destID) {
    auto connectedAreas = *(getAreaByID(sourceID)->getConnectedAreas());
    auto ptr = std::find_if(connectedAreas.begin(), connectedAreas.end(), [&destID] (const auto& pair) { return pair.second == destID; });
    return ptr == connectedAreas.end() ? "" : ptr->first;
}

std::string GameModel::getRelativeDirection(const std::string& direction) {
    if (direction == GameStringKeys::UP_DIRECTION) {
        return GameStrings::get(GameStringKeys::UP_RELATIVE_DIRECTION);
    } else if (direction == GameStringKeys::DOWN_DIRECTION) {
        return GameStrings::get(GameStringKeys::DOWN_RELATIVE_DIRECTION);
    } else if (direction == GameStringKeys::NORTH_DIRECTION) {
        return GameStrings::get(GameStringKeys::NORTH_RELATIVE_DIRECTION);
    } else if (direction == GameStringKeys::SOUTH_DIRECTION) {
        return GameStrings::get(GameStringKeys::SOUTH_RELATIVE_DIRECTION);
    } else if (direction == GameStringKeys::EAST_DIRECTION) {
        return GameStrings::get(GameStringKeys::EAST_RELATIVE_DIRECTION);
    } else if (direction == GameStringKeys::WEST_DIRECTION) {
        return GameStrings::get(GameStringKeys::WEST_RELATIVE_DIRECTION);
    }
    return direction;
}

void GameModel::sendMoveUpdateMessages(const std::string& playerID,
                                       const std::string& prevAreaID, const std::string& prevDir,
                                       const std::string& newAreaID, const std::string& newDir) {

    auto prevStrInfo = StringInfo{playerID, "", 0, prevDir};
    for (const auto& characterID : getCharacterIDsInArea(prevAreaID)) {
        auto character = getCharacterByID(characterID, false);
        character->pushToBuffer(GameStrings::getFormatted(GameStringKeys::PLAYER_LEAVE, prevStrInfo), GameStringKeys::MESSAGE_SENDER_SERVER, ColorTag::WHITE);
    }

    auto newStrInfo = StringInfo{playerID, "", 0, newDir};
    for (const auto& characterID : getCharacterIDsInArea(newAreaID)) {
        if (playerID != characterID) {
            auto character = getCharacterByID(characterID, false);
            std::string message = GameStrings::getFormatted(GameStringKeys::PLAYER_ENTER, newStrInfo);
            if (newDir.empty()) {
                message = GameStrings::getFormatted(GameStringKeys::PLAYER_ENTER_UNKNOWN, newStrInfo);
            }
            character->pushToBuffer(message, GameStringKeys::MESSAGE_SENDER_SERVER, ColorTag::WHITE);
        }
    }
}

bool GameModel::characterCanMove(const Character& character) {
    return character.getState() == CharacterState::IDLE;
}

bool GameModel::characterIsInCombat(const std::string& characterID) {
    auto character = getCharacterByID(characterID);
    return character->getState() == CharacterState::BATTLE;
}

bool GameModel::characterIsDead(const std::string& characterID) {
    auto character = getCharacterByID(characterID);
    return character->getState() == CharacterState::DEAD;
}

std::vector<std::string> GameModel::getPlayerIDsInArea(const std::string &areaID) const {
	Area* area = getAreaByID(areaID);

	std::vector<std::string> charactersInArea;

	//TODO consider keep track of character inside area class
	for (const auto& pair : characters) {
		auto character = pair.second;
		if (character.getAreaID() == areaID) {
			charactersInArea.push_back(character.getID());
		}
	}
	return charactersInArea;
}

std::vector<std::string> GameModel::getNPCIDsInArea(const std::string &areaID) const {
	std::vector<std::string>NPCsInArea;
	for (const auto& pair : npcs) {
		auto character = pair.second;
		if (character.getAreaID() == areaID) {
			NPCsInArea.push_back(character.getID());
		}
	}
	return NPCsInArea;
}

std::vector<std::string> GameModel::getCharacterIDsInArea(const std::string &areaID) const {
    std::vector<std::string> charactersInArea;
    auto playersIDs = getPlayerIDsInArea(areaID);
    auto npcsIDs = getNPCIDsInArea(areaID);

    charactersInArea.reserve(playersIDs.size() + npcsIDs.size());
    charactersInArea.insert(charactersInArea.end(), playersIDs.begin(), playersIDs.end());
    charactersInArea.insert(charactersInArea.end(), npcsIDs.begin(), npcsIDs.end());
    return charactersInArea;
}

/*
 *	LOCATION FUNCTIONS
 */

std::string GameModel::getDefaultLocationID() const {
	return this->defaultLocation;
}

void GameModel::setDefaultLocationID(const std::string& locationID) {
	this->defaultLocation = locationID;
}

Character* GameModel::getCharacterByID(const std::string& characterID) const {
	return getCharacterByID(characterID, true);
}

Character* GameModel::getCharacterByID(const std::string& characterID, bool considerStatusEffect) const {
	Character* character;
	if (characters.count(characterID) == 1) {
		character = (Character*)&(characters.at(characterID));
	}
	else if (npcs.count(characterID) > 0) {
		character = (NPC*)&(npcs.at(characterID));
	}
	else {
		return nullptr;
	}

	return (considerStatusEffect) ? getBodySwappedCharacter(character) : character;
}

NPC* GameModel::getNPCByID(const std::string& npcID) const {
	if (npcs.count(npcID) > 0) {
		return (NPC*)&(npcs.at(npcID));
	}
	return nullptr;
}

Character* GameModel::getBodySwappedCharacter(Character* character) const {

    auto& statusEffects = character->getStatusEffects();
    auto statusEffect = std::find_if(statusEffects.begin(), statusEffects.end(),
                                     [] (const auto& se) { return se->getType() == StatusType::BODYSWAP; });
    if (statusEffect == statusEffects.end()) {
        return character;
    } else {
        // TODO: print bodyswapped message
        auto characterID = std::static_pointer_cast<BodySwapStatus>(*statusEffect)->getSwappedID();
		if (characters.find(characterID) != characters.end()) {
			return (Character*)&(characters.at(characterID));
		}
		else if (npcs.find(characterID) != npcs.end()) {
			return (NPC*)&(npcs.at(characterID));
		}
		else return character;
    }
}

void GameModel::loadObjects(const YAML::Node& OBJECTS){
    this->objects = GameDataImporter::returnObjects(OBJECTS);
}

void GameModel::addObjectToAreas(){
    for ( const auto& reset : resets) {
        if (reset.getAction() == ACTION_OBJECT){

            //ActionID is ItemID
            std::string actionID = reset.getActionID();

            //find Objects
            std::string objectName;
            for(auto& object : objects){
                if(actionID == object.second.getID()) {
                    objectName = object.second.getName();
                }
            }
            std::string roomID = reset.getAreaID();

            //add to game model areas map
            locations.at(roomID).addObjects(objectName);
        }
    }
}

NPC* GameModel::getNPCInArea(const std::string& keyword, const std::string& areaID) {
	for (const auto& npcID: getNPCIDsInArea(areaID)) {
		auto npc = getNPCByID(npcID);
		for (auto& npcKeyword: npc->getKeywords()) {
			if (keyword == npcKeyword) {
				return npc;
			}
		}
	}
	return nullptr;
}

void GameModel::addNPCsToAreas() {
    for (const auto& reset : resets) {

        if (reset.getAction() == ACTION_NPC) {
            try {

                auto& npc = npcTemplates.at(reset.getActionID());
                for (int i = 0; i < reset.getLimit(); i++) {
                    std::string newNpcID = npc.getID();
//                    newNpcID = newNpcID.append(std::to_string(npc.getCounter()));
					newNpcID = npc.getName() + "-" + newNpcID.append(std::to_string(npc.getCounter()));

                    auto newNPC = NPC(npc);
                    newNPC.setID(newNpcID);
                    newNPC.setAreaID(reset.getAreaID());
                    npcs.insert(std::pair<std::string, NPC>(newNPC.getID(), newNPC));
                    npc.increaseCounter();
                }

            } catch (std::out_of_range ex) {

            }
        }
    }
}

void GameModel::setNPCs(const std::unordered_map<std::string, NPC> npcs) {
    this->npcTemplates = npcs;
}

void GameModel::setResets(const std::vector<Resets> resets) {
	this->resets = resets;
}

bool GameModel::engageCharacterInCombat(const std::string& characterID, const std::string& target) {
    auto c1 = getCharacterByID(characterID);
    auto c2 = getCharacterByID(target, true);

    if (c1 == nullptr || c2 == nullptr) {
        return false;
    }

    auto battleInstance = combatManager.getNewCombatInstance();
    battleInstance.addCharacterToNewTeam(*c1);
    battleInstance.addCharacterToNewTeam(*c2);
    combatManager.loadCombatInstance(battleInstance);
	auto targetName = getCharacterByID(target)->getName();
	auto stringInfo = StringInfo{characterID, targetName, 0, ""};
	c1->pushToBuffer(GameStrings::getFormatted(GameStringKeys::COMBAT_ENGAGE, stringInfo), GameStringKeys::MESSAGE_SENDER_BATTLE, ColorTag::WHITE);
	c2->pushToBuffer(GameStrings::getFormatted(GameStringKeys::COMBAT_ENGAGED, stringInfo), GameStringKeys::MESSAGE_SENDER_BATTLE, ColorTag::WHITE);

    return true;
}

bool GameModel::setCombatAction(const std::string& characterID, const std::string& actionName) {
    auto characterInstance = combatManager.getCharacterInstanceByCharacterID(characterID);
    if (characterInstance == nullptr) {
        return false;
    }
    characterInstance->setCombatActionID(actionName);
    return true;
}

bool GameModel::setCombatTarget(const std::string& characterID, const std::string& targetID) {
    auto combatInstance = combatManager.getCombatInstanceByCharacterID(characterID);

    if (combatInstance == nullptr) {
        return false;
    }

    auto characterInstance = combatInstance->getCharacterInstance(characterID);
    auto targetInstance = combatInstance->getCharacterInstance(targetID);

    if (targetInstance == nullptr) {
        return false;
    }

    characterInstance->setTarget(*targetInstance);
    return true;

}

std::vector<std::string> GameModel::getPossibleTargets(const std::string& characterID) {
    auto combatInstance = combatManager.getCombatInstanceByCharacterID(characterID);
    return combatInstance->getPossibleTargets(characterID);
}

void GameModel::update() {

    // Manage dead characters
    manageDeadCharacters();

    if (gameTicks % GameModel::GAME_TICKS_PER_COMBAT_TICK == 0) {
        // Update all the combat instances
        combatManager.update();
    }

    // Consider not calling this every tick
    updateStatusEffects();

    if (gameTicks % GameModel::GAME_TICKS_PER_NPC_TICK == 0) {
        runNPCScripts();
    }

    if (gameTicks % GameModel::GAME_TICKS_PER_SAVE_TICK == 0) {
        saveAllCharacters();
    }

    gameTicks++;
}

void GameModel::manageDeadCharacters() {
    for (auto& pair : characters) {
        auto& character = pair.second;
        if (character.getState() == CharacterState::DEAD) {
            character.setAreaID(getDefaultLocationID());
            character.setState(CharacterState::IDLE);
            character.setCurrentHealth(character.getMaxHealth());
            character.setCurrentMana(character.getMaxMana());
            character.getInventory().removeAllItems();
			character.pushToBuffer(GameStrings::get(GameStringKeys::PLAYER_RESPAWNED),
													GameStringKeys::MESSAGE_SENDER_SERVER, ColorTag::GREEN);
        }
    }

    for (auto it = npcs.begin(), ite = npcs.end(); it != ite;) {
        if (it->second.getState() == CharacterState::DEAD) {
            it = npcs.erase(it);
        } else {
            ++it;
        }
    }
}

// TODO: Move this dependency to different functions
void GameModel::loadActions(const std::unordered_map<std::string, std::shared_ptr<CombatAction>>& actionLookup) {
    combatManager.setActionLookup(actionLookup);
}

void GameModel::addSpell(Spell spell) {
    auto spellName = spell.getName();
    spells.insert({ spellName, std::move(spell) });
    Spell* spellRef = &(spells.at(spellName));
    combatManager.addSpellAction(*spellRef);
}

void GameModel::updateStatusEffects() {

    time_t currentTime;
    time(&currentTime);

    for (auto& pair : characters) {
		auto& character = pair.second;
		auto& statusEffects = character.getStatusEffects();
		removeExpiredStatus(currentTime, character, statusEffects);
	}
	for (auto& pair: npcs) {
		auto& npc = pair.second;
		auto& statusEffects = npc.getStatusEffects();
		removeExpiredStatus(currentTime, npc, statusEffects);
	}
}

void GameModel::removeExpiredStatus(time_t currentTime, Character &character,
									std::vector<std::shared_ptr<StatusEffect>> &statusEffects) const {
	auto eraseIter = remove_if(statusEffects.begin(), statusEffects.end(),
							   [&currentTime] (const auto& se) {
									   return se->getEndTime() < currentTime;
								   });
	if (eraseIter != statusEffects.end()) {
			character.pushToBuffer(GameStrings::get(GameStringKeys::STATUS_EFFECT_END), GameStringKeys::MESSAGE_SENDER_SERVER, ColorTag::WHITE);
		}
	statusEffects.erase(eraseIter, statusEffects.end());
}

void GameModel::pushToOutputBuffer(const std::string& characterID, std::string message, std::string sender, std::string color) {
	getCharacterByID(characterID)->pushToBuffer(message, sender, color);
}

void GameModel::sendGlobalMessage(const std::string& senderID, std::string message) {
	for (const auto& pair : characters) {
		auto character = pair.second;
		character.pushToBuffer(message, getCharacterByID(senderID)->getName(), ColorTag::WHITE);
	}
	for (const auto& pair : npcs) {
		auto character = pair.second;
		character.pushToBuffer(message, getCharacterByID(senderID)->getName(), ColorTag::WHITE);
	}
}

void GameModel::sendLocalMessageFromCharacter(const std::string& senderID, std::string message) {
	auto areaID = getCharacterByID(senderID)->getAreaID();
	for (const auto &character: getCharacterIDsInArea(areaID)) {
		getCharacterByID(character)->pushToBuffer(GameStrings::get(GameStringKeys::AREA_CHANNEL) + message,
                                                  getCharacterByID(senderID)->getName(), ColorTag::CYAN);
	}
}

void GameModel::sendPrivateMessage(const std::string& senderID, std::string message, const std::string& target) {
	auto targetCharacter = getCharacterByID(target);
	if (targetCharacter != nullptr) {
		targetCharacter->pushToBuffer(GameStrings::get(GameStringKeys::PRIVATE_CHANNEL) + message,
                                      getCharacterByID(senderID)->getName(), ColorTag::MAGENTA);
	}
}

void GameModel::listValidSpells(const std::string& characterID) {
	auto character = getCharacterByID(characterID);
	std::string message = GameStrings::get(GameStringKeys::SPELL_LIST) + "\n";
	for (auto& spellPairs: spells) {
		message += spellPairs.first + ", ";
	}
	character->pushToBuffer(message, GameStrings::get(GameStringKeys::SERVER_NAME), ColorTag::WHITE);
}

void GameModel::loadDefaultSpells() {
    Spell bodySwap("body swap", 0, SpellType::BODY_SWAP, "");
    addSpell(bodySwap);

    Spell pigLatin{"pig latin", 0, SpellType::PIG_LATIN, ""};
    addSpell(pigLatin);
}

void GameModel::castSpell(const std::string& sourceID, const std::string& targetID, const std::string& spellID) {
	auto spellsIter = spells.find(spellID);
	if (spellsIter != spells.end()) {
		if (characterIsInCombat(sourceID)) {
			//TODO: Handle switching targets based on targetID
			setCombatAction(sourceID, spellID);
			return;
		}

		auto& spell = spellsIter->second;
		if (spell.getType() == SpellType::OFFENSE && sourceID != targetID) {
			engageCharacterInCombat(sourceID, targetID);
			setCombatAction(sourceID, spellID);
		}
		else {
			CombatCast spellCast{spell};
			spellCast.execute(*getCharacterByID(sourceID), *getCharacterByID(targetID, (sourceID == targetID)));
		}
	} else {
		auto unknownSpellMessage = GameStrings::getFormatted(GameStringKeys::SPELL_UNKNOWN, StringInfo{sourceID, targetID, 0, spellID});
		getCharacterByID(sourceID)->pushToBuffer(unknownSpellMessage,
                                                 GameStrings::get(GameStringKeys::SERVER_NAME),
                                                 ColorTag::WHITE);
	}
}

void GameModel::runNPCScripts() {

    for (auto& pair : npcs) {
        auto& npc = pair.second;
        auto commands = npc.getCommandsToExecute();

        for (const auto& command : commands) {
            executeNPCCommand(npc.getID(), command);
        }
    }
}

void GameModel::executeNPCCommand(const std::string &npcID, const std::string &command) {
    std::stringstream ss;
    ss.str(command);
    std::string token;
    std::getline(ss, token, ' ');

    if (token == "say") {

        std::string message;
        std::getline(ss, message);

        if (message == "") {
            return;
        }

        sendLocalMessageFromCharacter(npcID, message);

    } else if (token == "mpechoat") {

        std::string target;
        std::getline(ss, target, ' ');

        std::string message;
        std::getline(ss, message);

        if (target == "" || message == "") {
            return;
        }

        sendPrivateMessage(npcID, message, target);
    }
}

void GameModel::saveAllCharacters() {
    for (const auto& pair : characters) {
        YmlSerializer::save_to_file(pair.second);
    }
}

const Character& GameModel::getCharacterBattleTarget(const std::string& characterID) {
    auto id = getCharacterByID(characterID)->getID();
    auto instance = combatManager.getCombatInstanceByCharacterID(id);
    auto playerInstance = instance->getCharacterInstance(id);
    auto targetInstance = playerInstance->getTarget();
    return targetInstance.getCharacterRef();
}

std::vector<Spell> GameModel::getAllSpells() {
    std::vector<Spell> spellList;
    spellList.reserve(spells.size());

    for (const auto& pair : spells) {
        spellList.emplace_back(pair.second);
    }

    return spellList;
}