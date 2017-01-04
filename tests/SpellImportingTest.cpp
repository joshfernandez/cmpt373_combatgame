#include <gtest/gtest.h>
#include <game/GameDataImporter.hpp>
#include <expr-evaluator/ExprEvaluator.hpp>

class SpellImportTest : public ::testing::Test {

protected:

    virtual void SetUp() {
        rootNode = GameDataImporter::getRootYAMLNode("./data/spells.yml");
    }

    YAML::Node rootNode;

};

TEST_F(SpellImportTest, ExprEvalBasicTest) {
    EXPECT_EQ(ExprEvaluator::evaluate_infix_1("2+2", 0), 4);
    EXPECT_EQ(ExprEvaluator::evaluate_infix_1("2+l", 2), 4);
    EXPECT_EQ(ExprEvaluator::evaluate_infix_1("(2-2)+2*2+2", 0), 6);
    EXPECT_EQ(ExprEvaluator::evaluate_infix_1("(2-2)+2*l+2", 2), 6);
}

TEST_F(SpellImportTest, SpellParse) {
    auto spells = GameDataImporter::getSpells(rootNode);
    GameModel gameModel;
    for (const auto& spell : spells) {
        gameModel.addSpell(spell);
    }
}
