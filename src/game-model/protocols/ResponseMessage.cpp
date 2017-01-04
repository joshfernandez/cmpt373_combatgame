#include "game/protocols/ResponseMessage.hpp"
#include "MessageSerializer.hpp"


namespace protocols
{

std::string serializeResponseMessage(const ResponseMessage& request)
{
    return serializeMessage<ResponseHeader>(request.header, request.body);
}

ResponseMessage deserializeResponseMessage(const std::string& serializedMessage)
{
    return deserializeMessage<ResponseMessage, ResponseHeader>(serializedMessage);
}

}