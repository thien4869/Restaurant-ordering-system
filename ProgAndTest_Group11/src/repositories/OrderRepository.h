
#ifndef ORDER_REPOSITORY_H
#define ORDER_REPOSITORY_H

#include <vector>
#include <string>
#include "../models/Order.h"

class OrderRepository {
private:
    std::vector<Order> orders;

public:
    // Add a new order
    void addOrder(const Order& order);

    // Get orders by customer email
    std::vector<Order> getOrdersByCustomer(
        const std::string& email
    ) const;

    // Find order by order number
    Order* getOrderByNumber(
        const std::string& orderNumber
    );

    // Get all orders (Admin)
    std::vector<Order> getAllOrders() const;

    // Update order status
    bool updateOrderStatus(
        const std::string& orderNumber,
        const std::string& status
    );
};

#endif
