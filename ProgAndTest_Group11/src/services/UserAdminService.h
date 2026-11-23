#ifndef USER_ADMIN_SERVICE_H
#define USER_ADMIN_SERVICE_H
#include "../repositories/UserRepository.h"
#include "../models/Auth.h"
#include <string>
#include <vector>

class UserAdminService
{
private:
    UserRepository &repo;
public:
    explicit UserAdminService(UserRepository &repository)
        : repo(repository) {}
    std::vector<User> getAllUsers(const Auth &auth) const;
    std::vector<User> searchUsers(
        const Auth &auth,
        const std::string &keyword) const;
    std::vector<User> getUsersByPage(
        const Auth &auth,
        int page,
        int pageSize) const;
    bool updateUser(
        const Auth &auth,
        const std::string &email,
        const std::string &name,
        const std::string &phone);
    bool deleteUser(
        const Auth &auth,
        const std::string &email);
};
#endif
