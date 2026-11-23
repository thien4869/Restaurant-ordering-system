
#include <iostream>
#include "../../src/repositories/UserRepository.h"

int main() {
    UserRepository repo;

    User user1;
    user1.name = "Nguyen Van A";
    user1.email = "user1@example.com";
    user1.phone = "0901234567";

    User user2;
    user2.name = "Tran Thi B";
    user2.email = "user2@example.com";
    user2.phone = "0912345678";

    bool added1 = repo.add(user1);
    bool added2 = repo.add(user2);

    std::vector<User> users = repo.getAllUsers();

    if (added1 && added2 && users.size() == 2) {
        std::cout << "[PASS] Get all users\n";
    } else {
        std::cout << "[FAIL] Get all users\n";
        return 1;
    }

    for (const User& user : users) {
        std::cout << "ID: " << user.id
                  << " | Name: " << user.name
                  << " | Email: " << user.email
                  << " | Phone: " << user.phone
                  << '\n';
    }

    return 0;
}

