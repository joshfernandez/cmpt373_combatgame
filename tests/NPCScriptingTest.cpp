#include <gtest/gtest.h>
#include <game/GameDataImporter.hpp>
#include <game/Character.hpp>
#include <game/NPCScripts.hpp>

class NPCScripting : public ::testing::Test {

protected:

    virtual void SetUp() {
        rootNode = GameDataImporter::getRootYAMLNode("./data/newgate.yml");
    }

    YAML::Node rootNode;

};


//TEST_F(NPCScripting, BasicTest) {
//    //NPC's are the value of an unordered map
//    auto NPCs = GameDataImporter::returnNPCS(rootNode);
//
//    for(auto SingleNPC : NPCs){
//        NPC currentNPC = SingleNPC.second;
//        std::vector<NPCScripts> NPCScript = currentNPC::getScripts();
//
//        for(auto script : NPCScript){
//            std::pair<std::string, std::vector<std::string>> qualifierCommands = script::getQualifierCommandsMap();
//
//            for(int i = 0; i < 2; i++){
//                std::cout << qualifierCommands.first << std::endl;
//                for(auto command : qualifierCommands.second){
//                    std:: cout << command << std::endl;
//                }
//            }
//        }
//    }
//}