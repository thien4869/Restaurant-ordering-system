#include <iostream>
#include <string>
#include "services/AccountService.h"
using namespace std;

void showCustomerMenu(AccountService& service, Auth& auth) {
    while (auth.isLoggedIn()) {
        cout << "\n========== CUSTOMER MENU ==========\n";
        cout << "Welcome, " << auth.user()->name << "\n";
        cout << "1. View profile\n";
        cout << "2. Update profile\n";
        cout << "3. Logout\n";
        cout << "===================================\n";
        cout << "Choose: ";
        string choice;
        getline(cin, choice);
        if (choice == "1") {
            cout << "\nName: " << auth.user()->name << "\n";
            cout << "Email: " << auth.user()->email << "\n";
            cout << "Phone: " << auth.user()->phone << "\n";
        }
        else if (choice == "2") {
            string name, phone, error;
            cout << "New name: ";
            getline(cin, name);
            cout << "New phone: ";
            getline(cin, phone);
            if (service.updateProfile(auth, name, phone, error)) {
                cout << "Profile updated successfully!\n";
            } else {
                cout << "Error: " << error << "\n";
            }
        }
        else if (choice == "3") {
            auth.logout();
            cout << "Logged out successfully!\n";
        }
        else {
            cout << "Invalid choice!\n";
        }
    }
}
void showAdminMenu(Auth& auth) {
    if (!auth.canAccessAdmin()) {
        cout << "Access denied!\n";
        return;
    }
    while (auth.isLoggedIn()) {
        cout << "\n============ ADMIN MENU ============\n";
        cout << "1. View account information\n";
        cout << "2. Logout\n";
        cout << "====================================\n";
        cout << "Choose: ";
        string choice;
        getline(cin, choice);
        if (choice == "1") {
            cout << "Admin: " << auth.user()->name << "\n";
            cout << "Email: " << auth.user()->email << "\n";
        }
        else if (choice == "2") {
            auth.logout();
            cout << "Admin logged out!\n";
        }
        else {
            cout << "Invalid choice!\n";
        }
    }
}
int main() {
    UserRepository userRepo;
    AccountService accountService(userRepo);
    Auth auth;
    while (true) {
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
        getline(cin, choice);
        if (!cin) {
            break;
        }
        if (choice == "1") {
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
            cout << "Phone number: ";
            getline(cin, phone);
            bool success = accountService.registerUser(
                name, email, password, confirm, phone, error
            );
            if (success) {
                cout << "Registration successful!\n";
            } else {
                cout << "Registration failed: " << error << "\n";
            }
        }
        else if (choice == "2") {
            string email, password, error;
            cout << "\n=== CUSTOMER LOGIN ===\n";
            cout << "Email: ";
            getline(cin, email);
            cout << "Password: ";
            getline(cin, password);
            if (accountService.login(email, password, auth, error)) {
                cout << "Login successful!\n";
                showCustomerMenu(accountService, auth);
            } else {
                cout << "Login failed: " << error << "\n";
            }
        }
        else if (choice == "3") {
            string email, password, error;
            cout << "\n=== ADMIN LOGIN ===\n";
            cout << "Email: ";
            getline(cin, email);
            cout << "Password: ";
            getline(cin, password);
            if (accountService.adminLogin(email, password, auth, error)) {
                cout << "Admin login successful!\n";
                showAdminMenu(auth);
            } else {
                cout << "Login failed: " << error << "\n";
            }
        }
        else if (choice == "0") {
            cout << "Goodbye!\n";
            break;
        }
        else {
            cout << "Invalid choice!\n";
        }
    }
    return 0;
}
