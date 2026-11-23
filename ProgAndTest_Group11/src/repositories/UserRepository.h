#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H
#include "../models/User.h"
#include <vector>
class UserRepository
{
    std::vector<User> users;
    int nextId = 1;
public:
    bool emailExists(const std::string &email) const;
    bool add(User user);
    User *findByEmail(const std::string &email);
    const User *findByEmail(const std::string &email) const;
    std::vector<User> getAllUsers() const;
    // TV5 - Update user
    bool updateUser(
        const std::string &email,
        const std::string &name,
        const std::string &phone);
    // TV5 - Search users
    std::vector<User> searchUsers(const std::string &keyword) const;
    // TV5 - Delete user
    bool deleteUser(const std::string &email);
    // TV5 - Pagination
    std::vector<User> getUsersByPage(int page, int pageSize) const;
};
#endif
