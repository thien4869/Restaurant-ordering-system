#include "integration/Tv1MenuBridge.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include "models/Auth.h"
#include "common/database/SqliteDatabase.h"
#include "repositories/MenuRepository.h"
#include "services/MenuService.h"
#include "presentation/customer/menu/MenuScreen.h"
#include "presentation/admin/dishes/DishManagementScreen.h"
#endif
namespace {

std::string readSqlFile(const std::string& filename) {
    std::ifstream file(
        std::filesystem::path(TV1_SOURCE_DIR) / "database" / filename
    );

    if (!file) {
        throw std::runtime_error(
            "Cannot open TV1 SQL file: " + filename
        );
    }

    std::ostringstream content;
    content << file.rdbuf();
    return content.str();
}

void initializeMenuDatabase(SqliteDatabase& database) {
    const auto tables = database.query(
        "SELECT COUNT(*) FROM sqlite_master "
        "WHERE type='table' AND name='categories'"
    );

    if (tables.empty() || tables[0].empty()) {
        throw std::runtime_error("Cannot inspect menu database.");
    }

    if (tables[0][0] == "0") {
        database.runMySqlScript(
            readSqlFile("schema_tv5_reference.sql")
        );
    }

    if (database.query(
            "SELECT COUNT(*) FROM categories")[0][0] == "0") {
        database.runScript(readSqlFile("seed_categories.sql"));
    }

    if (database.query(
            "SELECT COUNT(*) FROM products")[0][0] == "0") {
        database.runScript(readSqlFile("seed_menu.sql"));
    }
}
} 

void openTv1CustomerMenu() {
    try {
        SqliteDatabase database("restaurant_menu.db");
        initializeMenuDatabase(database);
        MenuRepository repository(database);
        MenuService service(repository);
        MenuScreen screen(service, std::cin, std::cout);
        screen.run();
    }
    catch (const std::exception& error) {
        std::cerr << "TV1 Menu error: " << error.what() << '\n';
    }
}
void openTv1AdminMenu(const Auth& auth) {
    if (!auth.canAccessAdmin()) {
        std::cout << "Access denied!\n";
        return;
    }
    try {
        SqliteDatabase database("restaurant_menu.db");
        initializeMenuDatabase(database);
        MenuRepository repository(database);
        MenuService service(repository);
        DishManagementScreen screen(service, std::cin, std::cout);
        screen.run(auth);
    }
    catch (const std::exception& error) {
        std::cerr << "TV1 Admin Menu error: "
                  << error.what() << '\n';
    }
}
