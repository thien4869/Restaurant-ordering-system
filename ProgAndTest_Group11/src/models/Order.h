
#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <cstdint>

class Order {
private:
    std::string orderNumber;
    std::string customerEmail;
    std::string orderDate;
    std::string status;
    std::string paymentType;
    std::string paymentStatus;
    std::int64_t totalAmount;

public:
    // Default constructor
    Order()
        : totalAmount(0) {}

    // Parameterized constructor
    Order(
        const std::string& orderNumber,
        const std::string& customerEmail,
        const std::string& orderDate,
        const std::string& status,
        const std::string& paymentType,
        const std::string& paymentStatus,
        std::int64_t totalAmount
    )
        : orderNumber(orderNumber),
          customerEmail(customerEmail),
          orderDate(orderDate),
          status(status),
          paymentType(paymentType),
          paymentStatus(paymentStatus),
          totalAmount(totalAmount) {}

    // Getters
    std::string getOrderNumber() const {
        return orderNumber;
    }

    std::string getCustomerEmail() const {
        return customerEmail;
    }

    std::string getOrderDate() const {
        return orderDate;
    }

    std::string getStatus() const {
        return status;
    }

    std::string getPaymentType() const {
        return paymentType;
    }

    std::string getPaymentStatus() const {
        return paymentStatus;
    }

    std::int64_t getTotalAmount() const {
        return totalAmount;
    }

    // Setters
    void setStatus(const std::string& status) {
        this->status = status;
    }

    void setPaymentStatus(const std::string& paymentStatus) {
        this->paymentStatus = paymentStatus;
    }

    void setTotalAmount(std::int64_t totalAmount) {
        this->totalAmount = totalAmount;
    }
};

#endif

