#include <iostream>
#include "../../src/repositories/UserRepository.h"
int main() {
    UserRepository repo;
    // 1. Tao nguoi dung
    User user;
    user.name = "Nguyen Van A";
    user.email = "user1@example.com";
    user.phone = "0901234567";
    if (!repo.add(user)) {
        std::cout << "[FAIL] Add test user\n";
        return 1;
    }
    // 2. Xoa nguoi dung da ton tai
    bool deleted = repo.deleteUser("user1@example.com");
    if (deleted &&
        repo.findByEmail("user1@example.com") == nullptr &&
        repo.getAllUsers().empty()) {
        std::cout << "[PASS] Delete user successfully\n";
    } else {
        std::cout << "[FAIL] Delete user\n";
        return 1;
    }
    // 3. Xoa nguoi dung khong ton tai
    bool notFound = repo.deleteUser("unknown@example.com");
    if (!notFound) {
        std::cout << "[PASS] User not found\n";
    } else {
        std::cout << "[FAIL] User not found\n";
        return 1;
    }
    // 4. Kiem tra email khong phan biet chu hoa/thuong
    repo.add(user);
    bool caseInsensitive = repo.deleteUser("USER1@EXAMPLE.COM");
    if (caseInsensitive && repo.getAllUsers().empty()) {
        std::cout << "[PASS] Case-insensitive email\n";
    } else {
        std::cout << "[FAIL] Case-insensitive email\n";
        return 1;
    }
    return 0;
}
