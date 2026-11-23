
#include <iostream>
#include "../services/OrderService.h"

void showOrderHistory(OrderService& orderService,
                      const std::string& customerEmail) {
    std::vector<Order> orders =
        orderService.getOrderHistory(customerEmail);

    std::cout << "\n=== ORDER HISTORY ===\n";

    if (orders.empty()) {
        std::cout << "No orders found.\n";
        return;
    }

    for (const Order& order : orders) {
        std::cout << "Order: " << order.getOrderNumber() << "\n";
        std::cout << "Status: " << order.getStatus() << "\n";
        std::cout << "--------------------\n";
    }
}

