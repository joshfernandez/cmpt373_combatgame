#ifndef loginsystem_hpp
#define loginsystem_hpp

#include <string>
#include <boost/optional.hpp>
#include "game/protocols/Authentication.hpp"
#include <map>
#include "AccountInfo.hpp"

template <class T>
struct AuthResult {
    T result;
    boost::optional<AccountInfo> account;
};

class Authenticator {
private:
    static void set_savefilevals(const std::string& user, const std::string& pass);
    static bool save_file_exists(const std::string& user);
    static std::string get_saveloc(const std::string& user);
    static std::map<std::string,std::string> create_savefiledata(const std::string& user, const std::string& pass);
    
public:
    static const int USERNAME_MAX_LENGTH = 10;
    
    static AuthResult<protocols::LoginResponseCode> login(const std::string& username, const std::string& password);
    static AuthResult<protocols::RegistrationResponseCode> registerAccount(const std::string& username,
                                                                           const std::string& password);
};

#endif /* loginsystem_hpp */
