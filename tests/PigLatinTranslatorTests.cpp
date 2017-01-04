#include <gtest/gtest.h>
#include <../include/PigLatinTranslator.hpp>



/***************************
 TEST CLASSES
***************************/



/*
class PigLatinTranslatorTests : public ::testing::Test {

    protected:

        virtual void SetUp() {
            rootNode = GameDataImporter::getRootYAMLNode("./data/spells.yml");
        }

        virtual void TearDown() {

        }


}; //PigLatinTranslatorTests class
*/


/***************************
 TESTS
***************************/
/*
    EXPECT_TRUE
    EXPECT_FALSE
    EXPECT_EQ(expected, actual)
    EXPECT_NE
    EXPECT_LT
    EXPECT_LE
    EXPECT_GT
    EXPECT_GE
    Note: Same for asserts
*/

static std::string defaultWord = "Bebinca";
static std::string defaultSentence = "The quick brown fox jumps over the lazy dog "
                                     "near the bank of the river.";
static std::string vowelStartingWord = "iguana";
static std::string consonantStartingWord = "panther";
static std::string capitalStartingWord = "Vancouver";
static std::string qStartingWord = "quiet";
static std::string contraction1 = "can't";
static std::string contraction2 = "it's";
static std::string contraction3 = "ain't";
static std::string hyphenated_word = "see-saw";
static std::string number = "123";
static std::string punctuation1 = ".";
static std::string punctuation2 = "&456";
static std::string alphanumeric = "abc123";
static std::string space = " ";
static std::string empty = "";



TEST(PigLatinTranslatorTests, normalCases) {
    EXPECT_EQ(translateToPigLatin(defaultWord), 
                "Ebincabay");
    EXPECT_EQ(translateToPigLatin(defaultSentence), 
                "Ethay ickquay ownbray oxfay umpsjay "
                "overway ethay azylay ogday earnay ethay "
                "ankbay ofway ethay iverray.");
}

TEST(PigLatinTranslatorTests, requiredCases) {
    EXPECT_EQ(translateToPigLatin(vowelStartingWord), 
                "iguanaway");
    EXPECT_EQ(translateToPigLatin(consonantStartingWord), 
                "antherpay");
    EXPECT_EQ(translateToPigLatin(capitalStartingWord), 
                "Ancouvervay");
    EXPECT_EQ(translateToPigLatin(contraction1), 
                "an'tcay");
    EXPECT_EQ(translateToPigLatin(contraction2), 
                "it'sway");
    EXPECT_EQ(translateToPigLatin(contraction3), 
                "ain'tway");
    EXPECT_EQ(translateToPigLatin(hyphenated_word), 
                "eesay-awsay");
}

TEST(PigLatinTranslatorTests, cornerCases) {
    EXPECT_EQ(translateToPigLatin(number), 
                "123");
    EXPECT_EQ(translateToPigLatin(punctuation1), 
                ".");
    EXPECT_EQ(translateToPigLatin(punctuation2), 
                "&456");
    EXPECT_EQ(translateToPigLatin(alphanumeric), 
                "abc123");
    EXPECT_EQ(translateToPigLatin(space), 
                " ");
    EXPECT_EQ(translateToPigLatin(empty), 
                "");
}