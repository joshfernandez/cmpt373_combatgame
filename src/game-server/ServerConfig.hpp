#ifndef ADVENTURE2016_SERVERCONFIG_HPP
#define ADVENTURE2016_SERVERCONFIG_HPP

#include <string>
#include "CommandConfig.hpp"
#include <boost/optional.hpp>

class ServerConfig {
public:
    ServerConfig(const std::string& configFileName);

    unsigned short getPort() const;
    unsigned int getTicksPerSecond() const;
    const std::string& getMapFilePath() const; //only one file for now
    const std::string& getSpellFilePath() const;
    const std::string& getLogDirectory() const;
    const std::string& getCommandConfigFile() const;
private:
    void loadConfigFile();

    std::string configFileName;

    unsigned short port;
    unsigned int ticks;
    std::string mapFilePath;
    std::string spellFilePath;
    std::string logDir;
    std::string commandConfigFile;
};


#endif //ADVENTURE2016_SERVERCONFIG_HPP
