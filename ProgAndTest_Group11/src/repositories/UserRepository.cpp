#include "UserRepository.h"
#include <algorithm>
#include <cctype>
static std::string norm(std::string s)
{
    for (char &c : s)
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}
bool UserRepository::emailExists(const std::string &email) const { return findByEmail(email) != nullptr; }
bool UserRepository::add(User user)
{
    if (emailExists(user.email))
        return false;
    user.id = nextId++;
    users.push_back(user);
    return true;
}
User *UserRepository::findByEmail(const std::string &email)
{
    for (auto &u : users)
        if (norm(u.email) == norm(email))
            return &u;
    return nullptr;
}
const User *UserRepository::findByEmail(const std::string &email) const
{
    for (const auto &u : users)
        if (norm(u.email) == norm(email))
            return &u;
    return nullptr;
}

// TV5 - Get all users
std::vector<User> UserRepository::getAllUsers() const
{
    return users;
}
// TV5 - Update user
bool UserRepository::updateUser(
    const std::string &email,
    const std::string &name,
    const std::string &phone)
{
    User *user = findByEmail(email);
    if (user == nullptr)
    {
        return false;
    }
    user->name = name;
    user->phone = phone;
    return true;
}
// TV5 - Search users
std::vector<User> UserRepository::searchUsers(
    const std::string &keyword) const
{
    std::vector<User> results;
    std::string key = norm(keyword);

    for (const User &user : users)
    {
        std::string name = norm(user.name);
        std::string email = norm(user.email);

        if (name.find(key) != std::string::npos ||
            email.find(key) != std::string::npos)
        {
            results.push_back(user);
        }
    }
    return results;
}
// TV5 - Delete user
bool UserRepository::deleteUser(const std::string &email)
{
    for (auto it = users.begin(); it != users.end(); ++it)
    {
        if (norm(it->email) == norm(email))
        {
            users.erase(it);
            return true;
        }
    }
    return false;
}
// TV5 - Pagination
std::vector<User> UserRepository::getUsersByPage(
    int page,
    int pageSize) const
{
    std::vector<User> result;

    if (page <= 0 || pageSize <= 0)
    {
        return result;
    }

    const size_t start =
        static_cast<size_t>(page - 1) *
        static_cast<size_t>(pageSize);

    if (start >= users.size())
    {
        return result;
    }

    const size_t end =
        std::min(start + static_cast<size_t>(pageSize), users.size());

    for (size_t i = start; i < end; ++i)
    {
        result.push_back(users[i]);
    }

    return result;
}