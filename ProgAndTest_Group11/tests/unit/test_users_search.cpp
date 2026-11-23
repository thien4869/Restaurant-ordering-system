#include <iostream>
#include "../../src/repositories/UserRepository.h"
int main() {
    UserRepository repo;
    User user1;
    user1.name = "Nguyen Van A";
    user1.email = "user1@example.com";
    User user2;
    user2.name = "Tran Thi B";
    user2.email = "user2@example.com";
    User user3;
    user3.name = "Nguyen Van C";
    user3.email = "user3@example.com";
    repo.add(user1);
    repo.add(user2);
    repo.add(user3);
    // Test 1: Tim theo ten
    auto result1 = repo.searchUsers("Nguyen");
    if (result1.size() == 2) {
        std::cout << "[PASS] Search by name\n";
    } else {
        std::cout << "[FAIL] Search by name\n";
        return 1;
    }
    // Test 2: Tim theo email
    auto result2 = repo.searchUsers("user2@example.com");
    if (result2.size() == 1 &&
        result2[0].name == "Tran Thi B") {
        std::cout << "[PASS] Search by email\n";
    } else {
        std::cout << "[FAIL] Search by email\n";
        return 1;
    }
    // Test 3: Khong phan biet chu hoa/thuong
    auto result3 = repo.searchUsers("NGUYEN");
    if (result3.size() == 2) {
        std::cout << "[PASS] Case-insensitive search\n";
    } else {
        std::cout << "[FAIL] Case-insensitive search\n";
        return 1;
    }
    // Test 4: Khong tim thay ket qua
    auto result4 = repo.searchUsers("unknown");
    if (result4.empty()) {
        std::cout << "[PASS] No matching users\n";
    } else {
        std::cout << "[FAIL] No matching users\n";
        return 1;
    }
    return 0;
}
