#ifndef USER_H
#define USER_H
#include <string>
struct User {
    int id = 0;
    std::string name, email, passwordHash, phone;
    std::string role = "customer";
};
#endif
