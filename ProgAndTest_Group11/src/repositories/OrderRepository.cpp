
#include "OrderRepository.h"

// Add a new order
void OrderRepository::addOrder(const Order& order) {
    orders.push_back(order);
}

// Get orders by customer email
std::vector<Order> OrderRepository::getOrdersByCustomer(
    const std::string& email
) const {
    std::vector<Order> result;

    for (const Order& order : orders) {
        if (order.getCustomerEmail() == email) {
            result.push_back(order);
        }
    }

    return result;
}

// Find order by order number
Order* OrderRepository::getOrderByNumber(
    const std::string& orderNumber
) {
    for (Order& order : orders) {
        if (order.getOrderNumber() == orderNumber) {
            return &order;
        }
    }

    return nullptr;
}

// Get all orders for Admin
std::vector<Order> OrderRepository::getAllOrders() const {
    return orders;
}

// Update order status
bool OrderRepository::updateOrderStatus(
    const std::string& orderNumber,
    const std::string& status
) {
    Order* order = getOrderByNumber(orderNumber);

    if (order == nullptr) {
        return false;
    }

    order->setStatus(status);
    return true;
}
