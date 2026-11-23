#include <iostream>
#include "../../src/repositories/UserRepository.h"
int main() {
    UserRepository repo;
    // 1. Tao nguoi dung
    User user;
    user.name = "Nguyen Van A";
    user.email = "user1@example.com";
    user.phone = "0901234567";
    repo.add(user);
    // 2. Cap nhat thong tin
    bool updated = repo.updateUser(
        "user1@example.com",
        "Nguyen Van B",
        "0987654321"
    );
    // 3. Kiem tra ket qua
    const User* result = repo.findByEmail("user1@example.com");
    if (updated && result != nullptr &&
        result->name == "Nguyen Van B" &&
        result->phone == "0987654321") {
        std::cout << "[PASS] Update user successfully\n";
    } else {
        std::cout << "[FAIL] Update user\n";
        return 1;
    }
    // 4. Kiem tra email khong ton tai
    bool notFound = repo.updateUser(
        "unknown@example.com",
        "Test User",
        "0123456789"
    );
    if (!notFound) {
        std::cout << "[PASS] User not found\n";
    } else {
        std::cout << "[FAIL] User not found\n";
        return 1;
    }
    return 0;
}

