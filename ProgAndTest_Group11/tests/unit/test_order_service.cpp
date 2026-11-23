
#include <iostream>
#include <cassert>
#include "../../src/services/OrderService.h"

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

    // Test 1: Customer views own order history
    assert(service.getOrderHistory("customer1@gmail.com").size() == 1);

    // Test 2: Customer cannot view another customer's order
    assert(service.getOrderDetails(
        "ORD-002", "customer1@gmail.com"
    ) == nullptr);

    // Test 3: Customer can view own order details
    assert(service.getOrderDetails(
        "ORD-001", "customer1@gmail.com"
    ) != nullptr);

    // Test 4: Customer can cancel a NEW order
    assert(service.cancelOrder(
        "ORD-001", "customer1@gmail.com"
    ));

    // Test 5: Customer cannot cancel a CONFIRMED order
    assert(!service.cancelOrder(
        "ORD-002", "customer2@gmail.com"
    ));

    // Test 6: Admin can view all orders
    assert(service.getAllOrdersForAdmin().size() == 2);

    // Test 7: Admin can update order status
    assert(service.updateOrderStatus("ORD-002", "COMPLETED"));

    // Test 8: Invalid status is rejected
    assert(!service.updateOrderStatus("ORD-002", "INVALID"));

    std::cout << "All Order Service unit tests passed!\n";

    return 0;
}
