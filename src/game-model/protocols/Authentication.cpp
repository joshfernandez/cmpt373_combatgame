#include "game/protocols/Authentication.hpp"
#include "game/protocols/RequestMessage.hpp"
#include "game/protocols/ResponseMessage.hpp"
#include "yaml-cpp/yaml.h"
#include "GameStrings.hpp"
#include <string>

namespace protocols {

const std::string USERNAME_KEY = "user";
const std::string PASSWORD_KEY = "pass";
const std::string MESSAGE_KEY = "err";
const std::string SUCCESS_KEY = "ok";

//using a switch instead of map so compiler can warn if we forgot a key
std::string getLoginReponseMessage(LoginResponseCode code)
{
    switch (code) {
        case LoginResponseCode::LOGIN_OK:
            return GameStrings::get(GameStringKeys::LOGIN_Ok);
        case LoginResponseCode::USERNAME_NOT_FOUND:
            return GameStrings::get(GameStringKeys::USER_NOT_FOUND);
        case LoginResponseCode::INVALID_CREDENTIALS:
            return GameStrings::get(GameStringKeys::INVALID_CREDENTIALS);
        case LoginResponseCode::USER_LOGGED_IN:
            return GameStrings::get(GameStringKeys::USER_LOGGED_IN);
    }
}

std::string getRegistrationMessage(RegistrationResponseCode code)
{
    switch (code) {
        case RegistrationResponseCode::REGISTRATION_OK:
            return GameStrings::get(GameStringKeys::REG_OK);
        case RegistrationResponseCode::USERNAME_EXISTS:
            return GameStrings::get(GameStringKeys::USER_EXISTS);
        case RegistrationResponseCode::USERNAME_TOO_LONG:
            return GameStrings::get(GameStringKeys::USERNAME_TOO_LONG);
        case RegistrationResponseCode::USERNAME_INVALID:
            return GameStrings::get(GameStringKeys::USERNAME_INVALD);
    }
}

//can be used for login and registration
inline YAML::Node createCredentialData(const std::string& username, const std::string password)
{
    YAML::Node data;
    data[USERNAME_KEY] = username;
    data[PASSWORD_KEY] = password;

    return data;
}

RequestMessage createLoginRequestMessage(const UserCredentials& credentials)
{
    auto body = createCredentialData(credentials.username, credentials.password);
    return RequestMessage{RequestHeader::LOGIN_REQUEST, body};
}

RequestMessage createRegistrationRequestMessage(const UserCredentials& credentials)
{
    auto body = createCredentialData(credentials.username, credentials.password);
    return RequestMessage{RequestHeader::REGISTER_REQUEST, body};
}

UserCredentials readAuthenticationRequestMessage(const RequestMessage& message)
{
    auto username = message.body[USERNAME_KEY].as<std::string>();
    auto password = message.body[PASSWORD_KEY].as<std::string>();
    return UserCredentials{username, password};
}

inline YAML::Node createResponsedata(bool success, const std::string message)
{
    YAML::Node data;
    data[SUCCESS_KEY] = success;
    data[MESSAGE_KEY] = message;

    return data;
}

ResponseMessage createLoginResponseMessage(LoginResponseCode code)
{
    bool success = (code == LoginResponseCode::LOGIN_OK);
    auto message = getLoginReponseMessage(code);

    return ResponseMessage{ResponseHeader::LOGIN_RESPONSE, createResponsedata(success, message)};
}

ResponseMessage createRegistrationResponseMessage(RegistrationResponseCode code)
{
    bool success = (code == RegistrationResponseCode::REGISTRATION_OK);
    auto message = getRegistrationMessage(code);

    return ResponseMessage{ResponseHeader::REGISTER_RESPONSE, createResponsedata(success, message)};
}

AuthenticationResponse readAuthenticationResponseMessage(const ResponseMessage& responseMessage)
{
    auto success = responseMessage.body[SUCCESS_KEY].as<bool>();
    auto message = responseMessage.body[MESSAGE_KEY].as<std::string>();

    return AuthenticationResponse{success, message};
}

}