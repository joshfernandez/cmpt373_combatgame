#include <glog/logging.h>
#include "game/protocols/DisplayMessage.hpp"

namespace protocols
{

const std::string MESSAGE_KEY = "msg";
const std::string SENDER_KEY = "sender";

ResponseMessage createDisplayResponseMessage(const DisplayMessage& message)
{
    YAML::Node data;
    data[MESSAGE_KEY] = message.message;

    if (message.sender) {
        data[SENDER_KEY] = message.sender.get();
    }

    return ResponseMessage{ResponseHeader::DISPLAY_MESSAGE_RESPONSE, data};
}


DisplayMessage readDisplayResponseMessage(const ResponseMessage& responseMessage)
{
    CHECK(responseMessage.header == ResponseHeader::DISPLAY_MESSAGE_RESPONSE) << "Invalid response header type";

    DisplayMessage msg;
    if (responseMessage.body[SENDER_KEY]) {
        auto sender = responseMessage.body[SENDER_KEY].as<std::string>();
        msg.sender = sender;
    }

    msg.message = responseMessage.body[MESSAGE_KEY].as<std::string>();

    return msg;
}



}