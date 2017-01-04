#include <glog/logging.h>
#include "game/protocols/Editing.hpp"
#include "game/GameDataImporter.hpp"

namespace {

const std::string EDIT_TYPE_TAG = "type";
const std::string SUCCESS_TAG = "ok";
const std::string MESSAGE_TAG = "msg";
const std::string ROOM_TAG = "room";

}

namespace protocols {

ResponseMessage createEditResponse(const EditResponse& editResponse) {
    YAML::Node node;
    node[EDIT_TYPE_TAG] = static_cast<int>(editResponse.editType);
    node[SUCCESS_TAG] = editResponse.success;
    node[MESSAGE_TAG] = editResponse.message;
    if (editResponse.data) {
        node[ROOM_TAG] = editResponse.data.get();
    }

    return ResponseMessage{header : ResponseHeader::EDIT_INFO_RESPONSE, body : node};
}

EditResponse readEditResponse(const ResponseMessage& responseMessage) {
    CHECK(responseMessage.header == ResponseHeader::EDIT_INFO_RESPONSE) << "Invalid response header type";

    EditResponse editResponse;
    editResponse.editType = static_cast<EditType>(responseMessage.body[EDIT_TYPE_TAG].as<int>());
    editResponse.success = responseMessage.body[SUCCESS_TAG].as<bool>();
    editResponse.message = responseMessage.body[MESSAGE_TAG].as<std::string>();

    auto data = responseMessage.body[ROOM_TAG];
    if (data) {
        editResponse.data = data.as<Area>();
    }

    return editResponse;
}

}