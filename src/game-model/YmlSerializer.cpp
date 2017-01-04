//
//
//

#include "YmlSerializer.hpp"


namespace {
    const std::string SAVE_LOCATION = "savefiles/";
    const std::string USERNAME_KEY = "username";
    const std::string PASSWORD_KEY = "password";
    const std::string ID = "id";
    const std::string NAME = "name";
    const std::string AREAID = "area_id";
    const std::string DAMAGE = "damage";
    const std::string ARMOR = "armor";
    const std::string GOLD = "gold";
    const std::string LEVEL = "level";
    const std::string CURRENTMANA = "current_mana";
    const std::string CURRENTHEALTH = "current_health";
    const std::string EXPERIENCE = "exp";
    const std::string MAXHEALTH = "max_health";
    const std::string MAXMANA = "max_mana";
}


std::string YmlSerializer::get_saveloc(const std::string& username){
    std::string send_loc = SAVE_LOCATION + username + ".yml";
    return send_loc;
}


Character YmlSerializer::load_from_file(const std::string &username) {

    YAML::Node user_yaml_file = YAML::LoadFile(get_saveloc(username));

    auto area_id = user_yaml_file[AREAID].as<std::string>();
    auto armor = user_yaml_file[ARMOR].as<int>();
    auto current_health = user_yaml_file[CURRENTHEALTH].as<int>();
    auto current_mana = user_yaml_file[CURRENTMANA].as<int>();
    auto damage = user_yaml_file[DAMAGE].as<std::string>();
    auto exp = user_yaml_file[EXPERIENCE].as<int>();
    auto gold = user_yaml_file[GOLD].as<int>();
    auto id = user_yaml_file[ID].as<std::string>();
    auto level = user_yaml_file[LEVEL].as<int>();
    auto name = user_yaml_file[NAME].as<std::string>();
    auto max_health = user_yaml_file[MAXHEALTH].as<int>();
    auto max_mana = user_yaml_file[MAXMANA].as<int>();

    Character c(username, username, Character::defaultHit, damage, level, exp, armor, gold, area_id);
    c.setCurrentHealth(current_health);
    c.setCurrentMana(current_mana);
    c.setMaxHealth(max_health);
    c.setMaxMana(max_mana);

    return c;


}

std::map<std::string, std::string> YmlSerializer::update_savefiledata(const Character& c) {

    std::map<std::string,std::string> ret_map = {
            {ID, c.getID()},
            {NAME, c.getName()},
            {AREAID, c.getAreaID()},
            {DAMAGE, c.getDamage()},
          //  {ARMOR, std::to_string(c.getArmor())}, causing compile issues check
            {GOLD, std::to_string(c.getGold())},
            {LEVEL, std::to_string(c.getLevel())},
            {CURRENTMANA, std::to_string(c.getCurrentMana())},
            {CURRENTHEALTH, std::to_string(c.getCurrentHealth())},
            {EXPERIENCE, std::to_string(c.getExp())},
            {MAXHEALTH, std::to_string(c.getMaxHealth())},
            {MAXMANA, std::to_string(c.getMaxMana())}
    };

    return ret_map;


}

void YmlSerializer::save_to_file(const Character &c) {

    YAML::Node user_update_file = YAML::LoadFile(get_saveloc(c.getName()));
    std::map<std::string,std::string> savefile_update_map = update_savefiledata(c);

    //Create emitter with key:values

    for (std::map<std::string, std::string>::iterator it = savefile_update_map.begin(); it!= savefile_update_map.end(); it++){
       user_update_file[it->first] = it->second;
    }


    std::ofstream f;
    f.open(get_saveloc(c.getName()));
    f <<user_update_file; //Dump contents as string
    f.close();



}