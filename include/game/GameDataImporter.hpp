#ifndef GAME_DATA_IMPORTER_HPP
#define GAME_DATA_IMPORTER_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <boost/algorithm/string/join.hpp>
#include "StringUtils.hpp"

#include "yaml-cpp/yaml.h"
#include <game/Character.hpp>
#include "../../src/game-model/Resets.hpp"

#include <game/Area.hpp>
#include <game/Spell.hpp>
#include <game/Object.hpp>
#include <game/GameModel.hpp>
#include <game/NPCScripts.hpp>

class GameDataImporter {

private:

	static bool tryParseSpell(const YAML::Node& SPELL, SpellType spellType, Spell& retSpell);

public:


	//Inputs .YAML file for parsing
    static YAML::Node getRootYAMLNode(const std::string& fileName);

    static std::vector<Area> getRooms(const YAML::Node& ROOMS);
	static std::unordered_map<std::string, Object> returnObjects(const YAML::Node& OBJECTS);
	static std::unordered_map<std::string, NPC> returnNPCS(const YAML::Node&);
	static std::vector<Resets> returnResets(const YAML::Node&);
	//static void loadShops(GameModel& gameModel, const YAML::Node&);


	//Spells
	static std::vector<Spell> getSpells(const YAML::Node&);


};

namespace YamlKeys {
const static std::string AREA_ID_KEY = "id";
const static std::string AREA_NAME_KEY = "name";
const static std::string AREA_DESC_KEY = "desc";
const static std::string AREA_EXTENDED_DESC_KEY = "extended_descriptions";
const static std::string AREA_DOORS_KEY = "doors";
const static std::string AREA_DOOR_DIR_KEY = "dir";
const static std::string AREA_DOOR_TO_KEY = "to";
const static std::string AREA_EXTENDED_KEYWORDS_KEY = "keywords";
}

namespace YAML {
template<>
struct convert<Area> {
	static Node encode(const Area& area) {
		Node node;
		node[YamlKeys::AREA_ID_KEY] = area.getID();
		node[YamlKeys::AREA_NAME_KEY] = area.getTitle();
		node[YamlKeys::AREA_DESC_KEY].push_back(area.getDescription());

		for (const auto& pair : area.getExtendedDescriptions()) {
		    YAML::Node desc;
			desc[YamlKeys::AREA_EXTENDED_KEYWORDS_KEY].push_back(pair.first);
			desc[YamlKeys::AREA_DESC_KEY].push_back(pair.second);
			node[YamlKeys::AREA_EXTENDED_DESC_KEY].push_back(desc);
		}

		for (const auto& door : *area.getConnectedAreas()) {
			YAML::Node doorNode;
			doorNode[YamlKeys::AREA_DOOR_DIR_KEY] = door.first;
			doorNode[YamlKeys::AREA_DOOR_TO_KEY] = door.second;

			node[YamlKeys::AREA_DOORS_KEY].push_back(doorNode);
		}

		return node;
	}

    static bool decode(const Node& node, Area& area) {
        auto desc = node[YamlKeys::AREA_DESC_KEY].as<std::vector<std::string>>();
        auto description = boost::algorithm::join(desc, " ");

        std::unordered_map<std::string, std::string> exKeywordToDesc;
		for (const auto& EX_DESC : node[YamlKeys::AREA_EXTENDED_DESC_KEY]) {
			auto ex_desc_list = EX_DESC[YamlKeys::AREA_DESC_KEY].as<std::vector<std::string>>();
			auto ex_desc = boost::join(ex_desc_list, " ");
			std::string ex_keyword = EX_DESC[YamlKeys::AREA_EXTENDED_KEYWORDS_KEY].as<std::vector<std::string>>()[0];
			exKeywordToDesc[ex_keyword] = ex_desc;
		}

        auto id = node[YamlKeys::AREA_ID_KEY].as<std::string>();
        auto name = node[YamlKeys::AREA_NAME_KEY].as<std::string>();

        std::unordered_map<std::string, std::string> doorsMap;
        for(const auto& door : node[YamlKeys::AREA_DOORS_KEY]){
            auto dir = door[YamlKeys::AREA_DOOR_DIR_KEY].as<std::string>();
            auto to = door[YamlKeys::AREA_DOOR_TO_KEY].as<std::string>();
            doorsMap.emplace(dir, to);
        }

        area = Area{id, name, doorsMap, description, exKeywordToDesc};
        return true;
    }
};
}

#endif