
#include "OrderService.h"

// Constructor
OrderService::OrderService(OrderRepository& repository)
    : orderRepository(repository) {}

// 1. Customer: View order history
std::vector<Order> OrderService::getOrderHistory(
    const std::string& customerEmail
) const {
    if (customerEmail.empty()) {
        return {};
    }

    return orderRepository.getOrdersByCustomer(customerEmail);
}

// 2. Customer: View order details
Order* OrderService::getOrderDetails(
    const std::string& orderNumber,
    const std::string& customerEmail
) {
    if (orderNumber.empty() || customerEmail.empty()) {
        return nullptr;
    }

    Order* order = orderRepository.getOrderByNumber(orderNumber);

    if (order == nullptr) {
        return nullptr;
    }

    // Customers can only view their own orders
    if (order->getCustomerEmail() != customerEmail) {
        return nullptr;
    }

    return order;
}

// 3. Customer: Cancel order
bool OrderService::cancelOrder(
    const std::string& orderNumber,
    const std::string& customerEmail
) {
    Order* order = getOrderDetails(orderNumber, customerEmail);

    if (order == nullptr) {
        return false;
    }

    // Only NEW orders can be cancelled
    if (order->getStatus() != "NEW") {
        return false;
    }

    return orderRepository.updateOrderStatus(
        orderNumber,
        "CANCELED"
    );
}

// 4. Admin: View all orders
std::vector<Order> OrderService::getAllOrdersForAdmin() const {
    return orderRepository.getAllOrders();
}

// 5. Admin: Update order status
bool OrderService::updateOrderStatus(
    const std::string& orderNumber,
    const std::string& status
) {
    if (orderNumber.empty()) {
        return false;
    }

    const std::vector<std::string> validStatuses = {
        "NEW",
        "CONFIRMED",
        "IN_TRANSIT",
        "COMPLETED",
        "CANCELED"
    };

    bool isValid = false;

    for (const std::string& validStatus : validStatuses) {
        if (status == validStatus) {
            isValid = true;
            break;
        }
    }

    if (!isValid) {
        return false;
    }

    return orderRepository.updateOrderStatus(
        orderNumber,
        status
    );
}

