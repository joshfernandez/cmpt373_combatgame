#ifndef ADVENTURE2016_MESSAGESERIALIZER_HPP
#define ADVENTURE2016_MESSAGESERIALIZER_HPP

#include "yaml-cpp/yaml.h"
#include "yaml-cpp/emitter.h"

const std::string MessageHeaderKey = "Header";
const std::string MessageBodyKey = "Body";

namespace protocols {

template<typename HeaderType>
static inline std::string serializeMessage(HeaderType header, const YAML::Node& body)
{
    YAML::Emitter output;

    output << YAML::BeginMap;
    output << YAML::Key << MessageHeaderKey;
    output << YAML::Value << header;

    output << YAML::Key << MessageBodyKey;
    output << YAML::Value << body;
    output << YAML::EndMap;

    return output.c_str();
}

template<typename T, typename HeaderType>
static inline T deserializeMessage(const std::string& serializedMessage)
{
    auto node = YAML::Load(serializedMessage);

    auto header = static_cast<HeaderType>(node[MessageHeaderKey].as<int>());
    auto body = node[MessageBodyKey];

    return T{header, body};
}

}

#endif //ADVENTURE2016_MESSAGESERIALIZER_HPP
