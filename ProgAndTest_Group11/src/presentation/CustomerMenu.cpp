
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

void showOrderDetails(OrderService& orderService,
                      const std::string& orderNumber,
                      const std::string& customerEmail) {
    Order* order = orderService.getOrderDetails(
        orderNumber, customerEmail
    );

    std::cout << "\n=== ORDER DETAILS ===\n";

    if (order == nullptr) {
        std::cout << "Order not found or access denied.\n";
        return;
    }

    std::cout << "Order Number: "
              << order->getOrderNumber() << "\n";

    std::cout << "Status: "
              << order->getStatus() << "\n";

    std::cout << "Payment Type: "
              << order->getPaymentType() << "\n";

    std::cout << "Payment Status: "
              << order->getPaymentStatus() << "\n";

void cancelCustomerOrder(OrderService& orderService,
                         const std::string& orderNumber,
                         const std::string& customerEmail) {
    bool success = orderService.cancelOrder(
        orderNumber, customerEmail
    );

    std::cout << "\n=== CANCEL ORDER ===\n";

    if (success) {
        std::cout << "Order canceled successfully.\n";
    } else {
        std::cout << "Cannot cancel order.\n";
        std::cout << "Order must belong to you "
                  << "and have status NEW.\n";
    }
}

    std::cout << "Total Amount: "
              << order->getTotalAmount() << " VND\n";
}

