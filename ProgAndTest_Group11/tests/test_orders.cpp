
#include <iostream>
#include <cassert>
#include "../src/services/OrderService.h"

int main() {
    OrderRepository repository;
    OrderService service(repository);

    // Create sample orders
    repository.addOrder(Order(
        "ORD-001", "customer1@gmail.com",
        "2026-11-23", "NEW",
        "CASH", "PENDING", 150000
    ));

    repository.addOrder(Order(
        "ORD-002", "customer2@gmail.com",
        "2026-11-23", "CONFIRMED",
        "ONLINE", "PAID", 200000
    ));

    // Test 1: View order history
    auto history =
        service.getOrderHistory("customer1@gmail.com");
    assert(history.size() == 1);

    // Test 2: View own order details
    Order* order = service.getOrderDetails(
        "ORD-001", "customer1@gmail.com"
    );
    assert(order != nullptr);

    // Test 3: Cannot view another customer's order
    assert(service.getOrderDetails(
        "ORD-002", "customer1@gmail.com"
    ) == nullptr);

    // Test 4: Cancel NEW order
    assert(service.cancelOrder(
        "ORD-001", "customer1@gmail.com"
    ));
    assert(order->getStatus() == "CANCELED");

    // Test 5: Cannot cancel CONFIRMED order
    assert(!service.cancelOrder(
        "ORD-002", "customer2@gmail.com"
    ));

    // Test 6: Admin views all orders
    assert(service.getAllOrdersForAdmin().size() == 2);

    // Test 7: Admin updates order status
    assert(service.updateOrderStatus(
        "ORD-002", "IN_TRANSIT"
    ));

    // Test 8: Reject invalid status
    assert(!service.updateOrderStatus(
        "ORD-002", "INVALID"
    ));

    std::cout << "All Orders tests passed!\n";
    return 0;
}
