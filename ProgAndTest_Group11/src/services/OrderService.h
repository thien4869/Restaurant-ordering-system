
#ifndef ORDER_SERVICE_H
#define ORDER_SERVICE_H

#include <vector>
#include <string>
#include "../repositories/OrderRepository.h"

class OrderService {
private:
    OrderRepository& orderRepository;

public:
    // Constructor
    OrderService(OrderRepository& repository);

    // 1. Get customer's order history
    std::vector<Order> getOrderHistory(
        const std::string& customerEmail
    ) const;

    // 2. Get order details
    Order* getOrderDetails(
        const std::string& orderNumber,
        const std::string& customerEmail
    );

    // 3. Cancel order
    bool cancelOrder(
        const std::string& orderNumber,
        const std::string& customerEmail
    );

    // 4. Admin: Get all orders
    std::vector<Order> getAllOrdersForAdmin() const;

    // 5. Admin: Update order status
    bool updateOrderStatus(
        const std::string& orderNumber,
        const std::string& status
    );
};

#endif

