#ifndef ADVENTURE2016_AUTH_H
#define ADVENTURE2016_AUTH_H

#include <string>
#include "RequestMessage.hpp"
#include "ResponseMessage.hpp"
#include "yaml-cpp/yaml.h"

namespace protocols {

struct UserCredentials {
    std::string username;
    std::string password;
};

enum class LoginResponseCode {
    LOGIN_OK,
    USERNAME_NOT_FOUND,
    INVALID_CREDENTIALS,
    USER_LOGGED_IN,
};


enum class RegistrationResponseCode {
    REGISTRATION_OK,
    USERNAME_TOO_LONG,
    USERNAME_EXISTS,
    USERNAME_INVALID,
};

struct AuthenticationResponse {
    bool success;
    std::string msg;
};

RequestMessage createLoginRequestMessage(const UserCredentials& credentials);
RequestMessage createRegistrationRequestMessage(const UserCredentials& credentials);
UserCredentials readAuthenticationRequestMessage(const RequestMessage& message);

ResponseMessage createLoginResponseMessage(LoginResponseCode code);
ResponseMessage createRegistrationResponseMessage(RegistrationResponseCode code);
AuthenticationResponse readAuthenticationResponseMessage(const ResponseMessage& responseMessage);

}

#endif //ADVENTURE2016_AUTH_H
