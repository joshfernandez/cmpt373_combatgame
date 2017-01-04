//
//  loginsystem.cpp
//  loginsystem
//

#include "Authenticator.hpp"
#include "GameStrings.hpp"
#include <glog/logging.h>
#include <fstream>
#include <sys/stat.h>
//#include <boost/assign.hpp>
//#include <boost/filesystem.hpp> does not compile



//Save file format(subject to change) remove comment after finalised


/*
 ------------------------------------------------------
 username.yml
 ------------------------------------------------------
 username:
 password:
 avatar type
 user level
 world
 */

namespace {
const std::string SAVE_LOCATION = "savefiles/";
const std::string USERNAME_KEY = "username";
const std::string PASSWORD_KEY = "password";
const std::string ROLE_FLAGS_KEY = "roleflags";
const int DEFAULT_ROLE_FLAGS = (int)PlayerRole::NORMAL;
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

std::string Authenticator::get_saveloc(const std::string& user){
    std::string send_loc = SAVE_LOCATION + user + ".yml";
    return send_loc;
}

AuthResult<protocols::LoginResponseCode> Authenticator::login(const std::string& username, const std::string& password) {
    if (username.length() > USERNAME_MAX_LENGTH) {
        return {protocols::LoginResponseCode::INVALID_CREDENTIALS};
    }

    std::string savefile_name = get_saveloc(username);
    if(!save_file_exists(savefile_name)){
        return {protocols::LoginResponseCode::USERNAME_NOT_FOUND};
    }

    YAML::Node user_yaml = YAML::LoadFile(savefile_name);
    auto file_user = user_yaml[USERNAME_KEY].as<std::string>();
    auto file_pass = user_yaml[PASSWORD_KEY].as<std::string>();
    auto roles_node = user_yaml[ROLE_FLAGS_KEY];
    int roles = DEFAULT_ROLE_FLAGS;
    if (!roles_node) {
        //Add versioning?
        LOG(WARNING) << "User:" << file_user << " is using old save file structure";
        set_savefilevals(file_user, file_pass);
    } else {
        roles = roles_node.as<int>();
    }


    if(file_user == username && file_pass == password){
        AccountInfo userAccount;
        userAccount.username = file_user;
        userAccount.playerRoleFlags = roles;
        return {protocols::LoginResponseCode::LOGIN_OK, userAccount};
    } else {
        return {protocols::LoginResponseCode::INVALID_CREDENTIALS};
    }

}

AuthResult<protocols::RegistrationResponseCode> Authenticator::registerAccount(const std::string& username,
                                                                               const std::string& password) {
    if (username.length() > USERNAME_MAX_LENGTH) {
        return {protocols::RegistrationResponseCode::USERNAME_TOO_LONG};
    }

    if (username == GameStrings::get(GameStringKeys::SERVER_NAME)) {
        return {protocols::RegistrationResponseCode::USERNAME_INVALID};
    }

    std::string savefile_name = get_saveloc(username);
    if(save_file_exists(savefile_name)){
        return {protocols::RegistrationResponseCode::USERNAME_EXISTS};
    }
    
    set_savefilevals(username, password);
    AccountInfo userAccount;
    userAccount.username = username;
    userAccount.playerRoleFlags = DEFAULT_ROLE_FLAGS;

    return {protocols::RegistrationResponseCode::REGISTRATION_OK, userAccount};
}

bool Authenticator::save_file_exists(const std::string& user){
    std::ifstream f(user);
    return f.good();
}

std::map<std::string, std::string> Authenticator::create_savefiledata(const std::string& user, const std::string& pass) {

    /*
        std::map<std::string,std::string> ret_map = boost::assign::map_list_of
        (USERNAME_KEY,user)
        (PASSWORD_KEY, pass)
        (ROLE_FLAGS_KEY, DEFAULT_ROLE_FLAGS)
        (ID, "0")
        (NAME, user)
        (AREAID, "-1")
        (DAMAGE, "1d7+2")
        (ARMOR, "0")
        (GOLD, "0")
        (LEVEL, "1")
        (CURRENTMANA, "100")
        (CURRENTHEALTH, "100")
        (EXPERIENCE, "0");

     */

    std::map<std::string,std::string> ret_map = {
            {USERNAME_KEY,user},
            {PASSWORD_KEY, pass},
            {ID, user},
            {NAME, user},
            {AREAID, "-1"},
            {DAMAGE, "1d7+2"},
            {ARMOR, "0"},
            {GOLD, "0"},
            {LEVEL, "1"},
            {CURRENTMANA, "100"},
            {CURRENTHEALTH, "100"},
            {EXPERIENCE, "0"},
            {MAXHEALTH, "100"},
            {MAXMANA, "100"}
    };

    return ret_map;


}

void Authenticator::set_savefilevals(const std::string& user, const std::string& pass){

    std::map<std::string,std::string> savefile_map = create_savefiledata(user,pass);

    //savefile_map[]

    /*
    const char save_path[] = "savefile/";

    boost::filesystem::path loc (save_path);

    if(!boost::filesystem::exists(loc)) {
        boost::filesystem::create_directory(loc);
    }

    */

    //Check for directory if does not exist then create it.
    struct stat stct = {0};
    auto location = SAVE_LOCATION.c_str();
    if(stat(location, &stct) == -1){
        mkdir(location,0700);
    }


    std::string savefile_name = get_saveloc(user);

    //Create emitter with key:values
    YAML::Emitter credentials_emitter;
    credentials_emitter << YAML::BeginMap;
    for (std::map<std::string, std::string>::iterator it = savefile_map.begin(); it!= savefile_map.end(); it++){
        credentials_emitter << YAML::Key     << it ->first;
        credentials_emitter << YAML::Value   << it -> second;
    }

    credentials_emitter <<YAML::Key << ROLE_FLAGS_KEY;
    credentials_emitter <<YAML::Value << DEFAULT_ROLE_FLAGS;

    credentials_emitter << YAML::EndMap;

    std::ofstream f;
    f.open(savefile_name);
    f << credentials_emitter.c_str(); //Dump contents as string
    f.close();

}


