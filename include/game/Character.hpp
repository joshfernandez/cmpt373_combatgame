#ifndef CHARACTER_H
#define CHARACTER_H

#include "Attributes.hpp"
#include "Inventory.hpp"
#include "GameStrings.hpp"
#include <game/StatusEffect.hpp>
#include <game/NPCScripts.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <memory>
#include <algorithm>
#include <deque>
#include <memory>

typedef std::shared_ptr<std::deque<PlayerMessage>> MessageBuffer;

enum CharacterState {
    IDLE,
    BATTLE,
    DEAD
};

class Character {
protected:
    std::string id;
    std::string name;
    std::string areaID;
    int level;
    int exp;
    std::string damage;
    int armor;
    // TODO: Fix this
    int maxHealth = 100;
    int currentHealth = 100;
    int maxMana = 100;
    int currentMana = 100;
    Inventory inventory;
    std::string hit;
    int gold;
    CharacterState state = CharacterState::IDLE;
	MessageBuffer outputBuffer;
    std::vector<std::shared_ptr<StatusEffect>> statusEffects;

public:
    static const int STARTING_LEVEL = 1;

    Character();

    Character(      const std::string& id,
                    const std::string& name,
                    const std::string& hit,
                    const std::string& damage,
                    int level,
                    int exp,
                    int armor,
                    int gold,
                    std::string& areaID
                    );

    std::string getName() const;
    std::string getID() const;
    std::string getAreaID() const;
    std::string getDamage() const;
    std::string getHit() const;
    int getLevel() const;
    int getExp() const;
    int getArmor() const;
    int getMaxHealth() const;
    int getCurrentHealth() const;
    int getMaxMana() const;
    int getCurrentMana() const;
    int getGold() const;
    Inventory& getInventory();
    CharacterState getState() const;
    std::vector<std::shared_ptr<StatusEffect>>& getStatusEffects();
	MessageBuffer getOutputBuffer();
	std::string getStatus();

    void setID(const std::string& id);
    void setName(const std::string& name);
    void setAreaID(const std::string& areaID);
    void setDamage(const std::string& damage);
    void setArmor(int armor);
    void setGold(int gold);
    void setInventory(const std::string& objectID);
    void setState(CharacterState state);
    void setLevel(int newLevel);
    void setMaxHealth(int maxHealth);
    void setCurrentHealth(int currentHealth);
    void setMaxMana(int maxMana);
    void setCurrentMana(int currentMana);
    void levelUp();
    void increaseExp(int expToAdd);

	void pushToBuffer(const std::string message, const std::string sender, std::string color);
    void addStatusEffect(std::shared_ptr<StatusEffect> statusEffect);
    bool hasStatusEffect(StatusType statusType);

    //Defaults
    static const std::string defaultID;
    static const std::string defaultName;
    static const std::string defaultHit;
    static const std::string defaultDamage;

    static const int defaultLevel = 1;
    static const int defaultExp = 0;
    static const int defaultArmor = 0;
    static const int defaultGold = 0;

private:

};


//NPC Subclass
class NPC : public Character{
public:
    NPC(const std::string &id,
		const std::string &name, //Name is the short desc
		std::string &hit,
		std::string &damage,
		int level,
		int exp,
		int armor,
		int gold,
		std::string &areaID,
		int thac0,
		const std::string &description,
		const std::vector<std::string> &keywords,
		const std::string &longDesc,
		std::unordered_map<std::string, NPCScripts> &scripts
	);

    std::string getDescription() const;
    const std::vector<std::string> & getKeywords() const;
    std::string getlongDesc() const;
    int getThac0() const;
    std::unordered_map<std::string, NPCScripts> getScripts() const;

    int getCounter() const;
    void increaseCounter();
    void setCounter(int newCount);

    std::vector<std::string> getCommandsToExecute();

    //Defaults
    static const std::vector<std::string> defaultDescription;
    static const std::vector<std::string> defaultKeywords;
    static const std::vector<std::string> defaultLongDescription;
    static const int defaultThac0 = 0;


private:
    std::string description;
    std::vector<std::string> keywords;
    std::string longDesc;
    int thac0;
    std::unordered_map<std::string, NPCScripts> scripts;
    int counter;

};

#endif
