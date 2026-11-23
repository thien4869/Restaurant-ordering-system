
#include <iostream>
#include "../services/OrderService.h"

void showAllOrders(OrderService& orderService) {
    std::vector<Order> orders =
        orderService.getAllOrdersForAdmin();

    std::cout << "\n=== ALL ORDERS ===\n";

    if (orders.empty()) {
        std::cout << "No orders found.\n";
        return;
    }

    for (const Order& order : orders) {
        std::cout << "Order Number: "
                  << order.getOrderNumber() << "\n";

        std::cout << "Customer Email: "
                  << order.getCustomerEmail() << "\n";

        std::cout << "Status: "
                  << order.getStatus() << "\n";

        std::cout << "Total Amount: "
                  << order.getTotalAmount() << " VND\n";

        std::cout << "--------------------\n";
    }
}

