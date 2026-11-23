#include <iostream>
#include "../../src/services/UserAdminService.h"
int main()
{
    UserRepository repo;
    UserAdminService service(repo);
    // Tao mot nguoi dung de kiem thu
    User user;
    user.name = "Nguyen Van A";
    user.email = "user1@example.com";
    user.phone = "0901234567";
    if (!repo.add(user))
    {
        std::cout << "[FAIL] Setup user\n";
        return 1;
    }
    // TEST 1: Admin duoc phep quan ly
    User admin;
    admin.role = "admin";
    Auth adminAuth;
    adminAuth.login(admin);
    if (service.getAllUsers(adminAuth).size() == 1 &&
        service.updateUser(
            adminAuth,
            "user1@example.com",
            "Nguyen Van B",
            "0987654321"))
    {
        std::cout << "[PASS] Admin access\n";
    }
    else
    {
        std::cout << "[FAIL] Admin access\n";
        return 1;
    }
    // TEST 2: Customer khong co quyen
    User customer;
    customer.role = "customer";
    Auth customerAuth;
    customerAuth.login(customer);
    bool customerDenied =
        service.getAllUsers(customerAuth).empty() &&
        service.searchUsers(customerAuth, "Nguyen").empty() &&
        !service.updateUser(
            customerAuth,
            "user1@example.com",
            "Unauthorized",
            "0000000000") &&
        !service.deleteUser(
            customerAuth,
            "user1@example.com");
    if (customerDenied)
    {
        std::cout << "[PASS] Customer denied\n";
    }
    else
    {
        std::cout << "[FAIL] Customer denied\n";
        return 1;
    }
    // TEST 3: Guest khong co quyen
    Auth guestAuth;
    bool guestDenied =
        service.getAllUsers(guestAuth).empty() &&
        service.getUsersByPage(guestAuth, 1, 10).empty() &&
        !service.deleteUser(
            guestAuth,
            "user1@example.com");
    if (guestDenied)
    {
        std::cout << "[PASS] Guest denied\n";
    }
    else
    {
        std::cout << "[FAIL] Guest denied\n";
        return 1;
    }
    // TEST 4: Admin duoc phep xoa
    if (service.deleteUser(adminAuth, "user1@example.com") &&
        repo.getAllUsers().empty())
    {
        std::cout << "[PASS] Admin delete\n";
    }
    else
    {
        std::cout << "[FAIL] Admin delete\n";
        return 1;
    }
    return 0;
}
