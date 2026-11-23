#ifndef AUTH_H
#define AUTH_H
#include "User.h"
#include <string>

class Auth
{
    bool loggedIn = false;
    User current;
public:
    void login(const User &user)
    {
        current = user;
        loggedIn = true;
    }
    void logout()
    {
        loggedIn = false;
        current = User{};
    }
    bool isLoggedIn() const
    {
        return loggedIn;
    }
    std::string role() const
    {
        return loggedIn ? current.role : "guest";
    }
    const User *user() const
    {
        return loggedIn ? &current : nullptr;
    }
    bool canAccessAdmin() const
    {
        return loggedIn && current.role == "admin";
    }
};
#endif
