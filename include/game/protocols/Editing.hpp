#ifndef ADVENTURE2016_WORLDBUILDING_HPP
#define ADVENTURE2016_WORLDBUILDING_HPP

#include <boost/optional.hpp>
#include <yaml-cpp/yaml.h>
#include "ResponseMessage.hpp"
#include "game/Area.hpp"

namespace protocols {

enum class EditType {
    AREA,
    SAVE_CHANGES,
};

struct EditResponse {
    EditType editType;
    bool success;
    std::string message;
    boost::optional<Area> data;
};

ResponseMessage createEditResponse(const EditResponse& editResponse);
EditResponse readEditResponse(const ResponseMessage& responseMessage);

}

#endif //ADVENTURE2016_WORLDBUILDING_HPP
