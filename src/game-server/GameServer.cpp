/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <boost/lexical_cast.hpp>
#include "glog/logging.h"
#include "ServerLoop.hpp"

std::string configFilePath = "config.yml";

int main(int argc, char *argv[]) {
    const std::string usage = "Searching for config.yml in current path. "
            "Pass the path of the config file as a command line argument if you want to use that instead.";

    if (argc < 2) {
        std::cout << usage << std::endl;
    } else {
        configFilePath = argv[1];
    }

    ServerConfig config{configFilePath};

    FLAGS_log_dir = config.getLogDirectory();
    google::InitGoogleLogging("GameServer");

    ServerLoop loop{config};
    Looper looper{config.getTicksPerSecond()};
    looper.run(loop);

    return 0;
}

