#include "UserAdminService.h"
// Xem danh sach nguoi dung
std::vector<User> UserAdminService::getAllUsers(
    const Auth &auth) const
{
    if (!auth.canAccessAdmin())
    {
        return {};
    }
    return repo.getAllUsers();
}
// Tim kiem nguoi dung
std::vector<User> UserAdminService::searchUsers(
    const Auth &auth,
    const std::string &keyword) const
{
    if (!auth.canAccessAdmin())
    {
        return {};
    }
    return repo.searchUsers(keyword);
}
// Phan trang
std::vector<User> UserAdminService::getUsersByPage(
    const Auth &auth,
    int page,
    int pageSize) const
{
    if (!auth.canAccessAdmin())
    {
        return {};
    }
    return repo.getUsersByPage(page, pageSize);
}
// Cap nhat nguoi dung
bool UserAdminService::updateUser(
    const Auth &auth,
    const std::string &email,
    const std::string &name,
    const std::string &phone)
{
    if (!auth.canAccessAdmin())
    {
        return false;
    }
    return repo.updateUser(email, name, phone);
}
// Xoa nguoi dung
bool UserAdminService::deleteUser(
    const Auth &auth,
    const std::string &email)
{
    if (!auth.canAccessAdmin())
    {
        return false;
    }
    return repo.deleteUser(email);
}
