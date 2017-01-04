#include <yaml-cpp/yaml.h>
#include <iostream>
#include "ServerConfig.hpp"

namespace {
const std::string PORT_KEY = "port";
const std::string TICKS_KEY = "ticks-per-second";
const std::string MAPFILE_KEY = "map-file";
const std::string SPELLFILE_KEY = "spell-file";
const std::string COMMANDS_KEY = "commands-file";
const std::string GLOG_DIR_KEY = "google-log-directory";

template<typename T>
T getConfigValueAs(const std::string& key, const YAML::Node& configRootNode) {
    if (!configRootNode[key]) {
        std::cerr << "Error. Configuration file does not contain the required key: " << key
                  << std::endl;
        exit(-1);
    }

    return configRootNode[key].as<T>();
}
}

unsigned short ServerConfig::getPort() const {
    return port;
}

const std::string& ServerConfig::getMapFilePath() const {
    return mapFilePath;
}

const std::string& ServerConfig::getSpellFilePath() const {
    return spellFilePath;
}

unsigned int ServerConfig::getTicksPerSecond() const {
    return ticks;
}


const std::string& ServerConfig::getLogDirectory() const {
    return logDir;
}

void ServerConfig::loadConfigFile() {
    try {
        YAML::Node root = YAML::LoadFile(configFileName);
        port = getConfigValueAs<unsigned short>(PORT_KEY, root);
        ticks = getConfigValueAs<unsigned int>(TICKS_KEY, root);
        mapFilePath = getConfigValueAs<std::string>(MAPFILE_KEY, root);
        spellFilePath = getConfigValueAs<std::string>(SPELLFILE_KEY, root);
        logDir = getConfigValueAs<std::string>(GLOG_DIR_KEY, root);
        commandConfigFile = getConfigValueAs<std::string>(COMMANDS_KEY, root);
    } catch (YAML::BadFile& exception) {
        std::cerr << "Not a valid server configuration file: " << configFileName << std::endl;
        exit(-1);
    }
}

ServerConfig::ServerConfig(const std::string& configFileName) : configFileName{configFileName} {
    loadConfigFile();
}

const std::string& ServerConfig::getCommandConfigFile() const {
    return commandConfigFile;
}



