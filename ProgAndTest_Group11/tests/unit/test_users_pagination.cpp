#include <iostream>
#include <string>
#include "../../src/repositories/UserRepository.h"

int main()
{
    UserRepository repo;

    // Tao 10 nguoi dung
    for (int i = 1; i <= 10; ++i)
    {
        User user;
        user.name = "User " + std::to_string(i);
        user.email = "user" + std::to_string(i) + "@example.com";

        if (!repo.add(user))
        {
            std::cout << "[FAIL] Add test user\n";
            return 1;
        }
    }

    // Test 1: Trang dau tien, moi trang 3 nguoi
    auto page1 = repo.getUsersByPage(1, 3);

    if (page1.size() == 3 &&
        page1[0].name == "User 1" &&
        page1[2].name == "User 3")
    {
        std::cout << "[PASS] First page\n";
    }
    else
    {
        std::cout << "[FAIL] First page\n";
        return 1;
    }

    // Test 2: Trang thu hai
    auto page2 = repo.getUsersByPage(2, 3);

    if (page2.size() == 3 &&
        page2[0].name == "User 4" &&
        page2[2].name == "User 6")
    {
        std::cout << "[PASS] Second page\n";
    }
    else
    {
        std::cout << "[FAIL] Second page\n";
        return 1;
    }

    // Test 3: Trang cuoi chi con 1 nguoi
    auto page4 = repo.getUsersByPage(4, 3);

    if (page4.size() == 1 &&
        page4[0].name == "User 10")
    {
        std::cout << "[PASS] Last page\n";
    }
    else
    {
        std::cout << "[FAIL] Last page\n";
        return 1;
    }

    // Test 4: Trang vuot qua so luong
    auto page5 = repo.getUsersByPage(5, 3);

    if (page5.empty())
    {
        std::cout << "[PASS] Page out of range\n";
    }
    else
    {
        std::cout << "[FAIL] Page out of range\n";
        return 1;
    }

    // Test 5: Tham so khong hop le
    auto invalidPage = repo.getUsersByPage(0, 3);
    auto invalidSize = repo.getUsersByPage(1, 0);

    if (invalidPage.empty() && invalidSize.empty())
    {
        std::cout << "[PASS] Invalid pagination parameters\n";
    }
    else
    {
        std::cout << "[FAIL] Invalid pagination parameters\n";
        return 1;
    }

    return 0;
}
