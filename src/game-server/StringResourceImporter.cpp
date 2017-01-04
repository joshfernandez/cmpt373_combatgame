#include <iostream>
#include "StringResourceImporter.hpp"



std::string StringResourceImporter::getString(const std::string& key){
    if (!stringNode[key]) {
        std::cerr << "ERROR: Corresponding string not Found for " << key << std::endl;
        exit(-1);
    }
    return stringNode[key].as<std::string>();
}


StringResourceImporter::StringResourceImporter(const std::string &stringResourcePath) {
    const auto stringFile = YAML::LoadFile(stringResourcePath);
    StringResourceImporter::stringNode = stringFile["strings"];
    if (!stringNode) {
        std::cerr << "ERROR: String file not found" << std::endl;
        exit(-1);
    }
}


