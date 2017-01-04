#include "EditMessageBuilder.hpp"
#include "game/protocols/Editing.hpp"

using namespace protocols;

EditMessageBuilder::EditMessageBuilder(const networking::Connection& client, const protocols::EditResponse& editResponse)
        : client{client},
          response{editResponse} {}

std::vector<MessageInfo> EditMessageBuilder::buildMessages() const {
    auto responseMessage = createEditResponse(response);
    return {MessageInfo{client : client, message : responseMessage}};
}
