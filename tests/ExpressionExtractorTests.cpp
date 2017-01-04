#include <gtest/gtest.h>
#include "../src/game-model/ExpressionExtractor.hpp"

static std::string createInput(const std::string expression) {
    return "'" + expression + "'";
}

TEST(ExpressionExtractorTest, NormalInput) {
    const std::string expr = "l*2+2";
    const std::string input = createInput(expr);
    std::string out;

    EXPECT_TRUE(tryExtractEffectsExpression(input, out));
    EXPECT_TRUE(out == expr);
}

TEST(ExpressionExtractorTest, EmptyInput) {
    const std::string expr = "";
    const std::string input = createInput(expr);
    std::string out;

    EXPECT_FALSE(tryExtractEffectsExpression(input, out));
}