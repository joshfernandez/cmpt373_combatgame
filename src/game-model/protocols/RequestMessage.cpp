#include <yaml-cpp/yaml.h>
#include "game/protocols/RequestMessage.hpp"
#include "MessageSerializer.hpp"

namespace protocols
{

std::string serializeRequestMessage(const RequestMessage& request)
{
    return serializeMessage<RequestHeader>(request.header, request.body);
}

RequestMessage deserializeRequestMessage(const std::string& serializedMessage)
{
    return deserializeMessage<RequestMessage, RequestHeader>(serializedMessage);
}

}