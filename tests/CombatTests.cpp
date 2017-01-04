#include <gtest/gtest.h>
#include <game/GameModel.hpp>


static const std::string CHAR1_ID = "1";
static const std::string CHAR2_ID = "2";

using namespace std;

class CombatTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        unordered_map<string, std::shared_ptr<CombatAction>> map;
        std::shared_ptr<CombatAttack> a = make_shared<CombatAttack>();
        map.insert({ a->getID(), a });

        gameModel.loadActions(map);

        Spell spell = Spell("spell", 1, SpellType::OFFENSE, "l");
        gameModel.addSpell(spell);

        gameModel.createCharacter(CHAR1_ID);
        gameModel.createCharacter(CHAR2_ID);

    }

    GameModel gameModel;
};

TEST_F(CombatTest, basicCombat) {
    gameModel.engageCharacterInCombat(CHAR1_ID, CHAR2_ID);
    EXPECT_TRUE(gameModel.characterIsInCombat(CHAR1_ID));
    EXPECT_TRUE(gameModel.characterIsInCombat(CHAR2_ID));

    while (gameModel.characterIsInCombat(CHAR1_ID)) {
        gameModel.update();
    }
    EXPECT_FALSE(gameModel.characterIsInCombat(CHAR2_ID));

    EXPECT_FALSE(gameModel.characterIsDead(CHAR1_ID));
    EXPECT_TRUE(gameModel.characterIsDead(CHAR2_ID));

    //should respawn
    gameModel.update();
    EXPECT_FALSE(gameModel.characterIsDead(CHAR1_ID));
    EXPECT_FALSE(gameModel.characterIsDead(CHAR2_ID));
}

TEST_F(CombatTest, spellCombat) {
    gameModel.engageCharacterInCombat(CHAR1_ID, CHAR2_ID);
    gameModel.setCombatAction(CHAR2_ID, "spell");
    EXPECT_TRUE(gameModel.characterIsInCombat(CHAR1_ID));
    EXPECT_TRUE(gameModel.characterIsInCombat(CHAR2_ID));

    while (gameModel.characterIsInCombat(CHAR1_ID)) {
        gameModel.update();
    }
    EXPECT_FALSE(gameModel.characterIsInCombat(CHAR2_ID));

    EXPECT_FALSE(gameModel.characterIsDead(CHAR1_ID));
    EXPECT_TRUE(gameModel.characterIsDead(CHAR2_ID));

    //should respawn
    gameModel.update();
    EXPECT_FALSE(gameModel.characterIsDead(CHAR1_ID));
    EXPECT_FALSE(gameModel.characterIsDead(CHAR2_ID));
}