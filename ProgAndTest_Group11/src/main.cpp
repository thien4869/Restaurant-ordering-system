#include <iostream>
#include <string>
#include <functional>
#include <sstream>
#include "services/AccountService.h"
#include "services/OrderService.h"
#include "services/UserAdminService.h"
#include "integration/Tv1MenuBridge.h"
using namespace std;

void showOrderHistory(OrderService &, const string &);
void showOrderDetails(OrderService &, const string &, const string &);
void cancelCustomerOrder(OrderService &, const string &, const string &);
void showAllOrders(OrderService &);
void updateAdminOrderStatus(OrderService &, const string &, const string &);
void createDemoAdmin(UserRepository &repo)
{
    User admin;
    admin.name = "Demo Administrator";
    admin.email = "admin@demo.com";
    admin.phone = "0900000000";
    admin.role = "admin";
    ostringstream hash;
    hash << hex << std::hash<string>{}("Admin12345");
    admin.passwordHash = hash.str();
    repo.add(admin);
}
void customerMenu(
    AccountService &accountService,
    OrderService &orderService,
    OrderRepository &orderRepo,
    Auth &auth,
    int &orderCounter)
{
    while (auth.isLoggedIn())
    {
        cout << "\n========== CUSTOMER MENU ==========\n";
        cout << "Welcome, " << auth.user()->name << "\n";
        cout << "1. View profile\n";
        cout << "2. Update profile\n";
        cout << "3. View order history\n";
        cout << "4. View order details\n";
        cout << "5. Cancel order\n";
        cout << "6. Create demo order (testing)\n";
        cout << "7. View restaurant menu\n";
        cout << "8. Logout\n";
        cout << "===================================\n";
        cout << "Choose: ";
        string choice;
        if (!getline(cin, choice))
            return;
        const string email = auth.user()->email;
        if (choice == "1")
        {
            cout << "Name: " << auth.user()->name << "\n";
            cout << "Email: " << email << "\n";
            cout << "Phone: " << auth.user()->phone << "\n";
        }
        else if (choice == "2")
        {
            string name, phone, error;
            cout << "New name: ";
            getline(cin, name);
            cout << "New phone: ";
            getline(cin, phone);
            if (accountService.updateProfile(auth, name, phone, error))
                cout << "Profile updated successfully!\n";
            else
                cout << "Error: " << error << "\n";
        }
        else if (choice == "3")
        {
            showOrderHistory(orderService, email);
        }
        else if (choice == "4")
        {
            string orderNumber;
            cout << "Enter order number: ";
            getline(cin, orderNumber);
            showOrderDetails(orderService, orderNumber, email);
        }
        else if (choice == "5")
        {
            string orderNumber;
            cout << "Enter order number to cancel: ";
            getline(cin, orderNumber);
            cancelCustomerOrder(orderService, orderNumber, email);
        }
        else if (choice == "6")
        {
            string orderNumber = "ORD" + to_string(orderCounter++);
            Order demoOrder(
                orderNumber,
                email,
                "2026-11-23",
                "NEW",
                "CASH",
                "UNPAID",
                150000);
            orderRepo.addOrder(demoOrder);
            cout << "Demo order created!\n";
            cout << "Order number: " << orderNumber << "\n";
            cout << "Total: 150000 VND\n";
        }
        else if (choice == "7")
        {
            openTv1CustomerMenu();
        }
        else if (choice == "8")
        {
            auth.logout();
            cout << "Logged out successfully!\n";
        }
        else
        {
            cout << "Invalid choice!\n";
        }
    }
}

void adminMenu(OrderService &orderService,
               UserAdminService &userAdminService,
               Auth &auth)
{
    if (!auth.canAccessAdmin())
    {
        cout << "Access denied!\n";
        return;
    }

    auto printUsers = [](const vector<User> &users)
    {
        if (users.empty())
        {
            cout << "No users found.\n";
            return;
        }

        for (const User &user : users)
        {
            cout << "Name: " << user.name << "\n";
            cout << "Email: " << user.email << "\n";
            cout << "Phone: " << user.phone << "\n";
            cout << "Role: " << user.role << "\n";
            cout << "-------------------------\n";
        }
    };

    while (auth.isLoggedIn() && auth.canAccessAdmin())
    {
        cout << "\n============ ADMIN MENU ============\n";
        cout << "1. View admin information\n";
        cout << "2. View all orders\n";
        cout << "3. Update order status\n";
        cout << "4. View all users\n";
        cout << "5. Search users\n";
        cout << "6. Update user\n";
        cout << "7. Delete user\n";
        cout << "8. View users by page\n";
        cout << "9. Manage dishes\n";
        cout << "0. Logout\n";
        cout << "====================================\n";
        cout << "Choose: ";

        string choice;
        if (!getline(cin, choice))
            return;

        if (choice == "1")
        {
            cout << "Name: " << auth.user()->name << "\n";
            cout << "Email: " << auth.user()->email << "\n";
        }
        else if (choice == "2")
        {
            showAllOrders(orderService);
        }
        else if (choice == "3")
        {
            string number, status;

            cout << "Order number: ";
            getline(cin, number);

            cout << "New status: ";
            getline(cin, status);

            updateAdminOrderStatus(orderService, number, status);
        }
        else if (choice == "4")
        {
            printUsers(userAdminService.getAllUsers(auth));
        }
        else if (choice == "5")
        {
            string keyword;

            cout << "Search keyword: ";
            getline(cin, keyword);

            printUsers(userAdminService.searchUsers(auth, keyword));
        }
        else if (choice == "6")
        {
            string email, name, phone;

            cout << "User email: ";
            getline(cin, email);
            cout << "New name: ";
            getline(cin, name);
            cout << "New phone: ";
            getline(cin, phone);

            if (userAdminService.updateUser(
                    auth, email, name, phone))
            {
                cout << "User updated successfully!\n";
            }
            else
            {
                cout << "Failed to update user.\n";
            }
        }
        else if (choice == "7")
        {
            string email;

            cout << "Email to delete: ";
            getline(cin, email);

            if (email == auth.user()->email)
            {
                cout << "Cannot delete your current account.\n";
                continue;
            }

            string confirm;
            cout << "Confirm delete (YES): ";
            getline(cin, confirm);

            if (confirm != "YES")
            {
                cout << "Deletion canceled.\n";
                continue;
            }

            if (userAdminService.deleteUser(auth, email))
            {
                cout << "User deleted successfully!\n";
            }
            else
            {
                cout << "Failed to delete user.\n";
            }
        }
        else if (choice == "8")
        {
            string input;
            int page = 0;

            cout << "Page number: ";
            getline(cin, input);

            try
            {
                size_t pos = 0;
                page = stoi(input, &pos);
                if (pos != input.size() || page < 1)
                {
                    throw invalid_argument("Invalid page");
                }
            }
            catch (const exception &)
            {
                cout << "Invalid page number.\n";
                continue;
            }

            printUsers(userAdminService.getUsersByPage(
                auth, page, 5));
        }
        else if (choice == "9")
        {
            openTv1AdminMenu(auth);
        }
        else if (choice == "0")
        {
            auth.logout();
            cout << "Admin logged out!\n";
        }
        else
        {
            cout << "Invalid choice!\n";
        }
    }
}
int main()
{
    UserRepository userRepo;
    AccountService accountService(userRepo);
    UserAdminService userAdminService(userRepo);
    OrderRepository orderRepo;
    OrderService orderService(orderRepo);
    Auth auth;
    int orderCounter = 1;
    createDemoAdmin(userRepo);
    while (true)
    {
        cout << "\n====================================\n";
        cout << "       RESTAURANT ORDERING SYSTEM\n";
        cout << "====================================\n";
        cout << "1. Register customer\n";
        cout << "2. Login customer\n";
        cout << "3. Login admin\n";
        cout << "0. Exit\n";
        cout << "====================================\n";
        cout << "Choose: ";
        string choice;
        if (!getline(cin, choice))
            break;
        if (choice == "1")
        {
            string name, email, password, confirm, phone, error;
            cout << "\n=== REGISTER CUSTOMER ===\n";
            cout << "Full name: ";
            getline(cin, name);
            cout << "Email: ";
            getline(cin, email);
            cout << "Password: ";
            getline(cin, password);
            cout << "Confirm password: ";
            getline(cin, confirm);
            cout << "Phone: ";
            getline(cin, phone);
            if (accountService.registerUser(
                    name, email, password, confirm, phone, error))
            {
                cout << "Registration successful!\n";
            }
            else
            {
                cout << "Registration failed: " << error << "\n";
            }
        }
        else if (choice == "2" || choice == "3")
        {
            string email, password, error;
            cout << "Email: ";
            getline(cin, email);
            cout << "Password: ";
            getline(cin, password);
            bool success = choice == "2"
                               ? accountService.login(email, password, auth, error)
                               : accountService.adminLogin(email, password, auth, error);
            if (!success)
            {
                cout << "Login failed: " << error << "\n";
                continue;
            }
            cout << "Login successful!\n";
            if (auth.canAccessAdmin())
            {
                adminMenu(orderService, userAdminService, auth);
            }
            else
            {
                customerMenu(
                    accountService,
                    orderService,
                    orderRepo,
                    auth,
                    orderCounter);
            }
        }
        else if (choice == "0")
        {
            cout << "Goodbye!\n";
            break;
        }
        else
        {
            cout << "Invalid choice!\n";
        }
    }
    return 0;
}
