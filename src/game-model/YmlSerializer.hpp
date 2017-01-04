//
//
//

#ifndef ADVENTURE2016_YMLSERIALIZER_HPP
#define ADVENTURE2016_YMLSERIALIZER_HPP

#include <iostream>
#include "yaml-cpp/yaml.h"
#include "game/Character.hpp"
#include <string>
#include <glog/logging.h>
#include <fstream>
#include <map>

class YmlSerializer {

private:
    static std::string get_saveloc(const std::string& username);
    static std::map<std::string, std::string> update_savefiledata(const Character& c);

public:
    static Character load_from_file(const std::string& username);
    static void save_to_file (const Character& c);


};


#endif //ADVENTURE2016_YMLSERIALIZER_HPP
