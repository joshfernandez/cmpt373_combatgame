#ifndef ADVENTURE2016_DISPLAYMESSAGE_HPP
#define ADVENTURE2016_DISPLAYMESSAGE_HPP

#include <string>
#include "RequestMessage.hpp"
#include "ResponseMessage.hpp"
#include "yaml-cpp/yaml.h"
#include "boost/optional.hpp"

namespace protocols {
//for now just a string, later on there will  be other stuff such as formatting and stuff
struct DisplayMessage {
    std::string message;
    boost::optional<std::string> sender;
};

ResponseMessage createDisplayResponseMessage(const DisplayMessage& message);
DisplayMessage readDisplayResponseMessage(const ResponseMessage& responseMessage);

}

#endif //ADVENTURE2016_DISPLAYMESSAGE_HPP
