
#ifndef ORDER_DETAIL_H
#define ORDER_DETAIL_H

#include <string>
#include <cstdint>

class OrderDetail {
private:
    std::string orderNumber;
    std::string productCode;
    int qty;
    std::int64_t price;

public:
    // Default constructor
    OrderDetail()
        : qty(0), price(0) {}

    // Parameterized constructor
    OrderDetail(
        const std::string& orderNumber,
        const std::string& productCode,
        int qty,
        std::int64_t price
    )
        : orderNumber(orderNumber),
          productCode(productCode),
          qty(qty),
          price(price) {}

    // Getters
    std::string getOrderNumber() const {
        return orderNumber;
    }

    std::string getProductCode() const {
        return productCode;
    }

    int getQty() const {
        return qty;
    }

    std::int64_t getPrice() const {
        return price;
    }

    // Setters
    void setQty(int qty) {
        this->qty = qty;
    }

    void setPrice(std::int64_t price) {
        this->price = price;
    }

    // Calculate subtotal
    std::int64_t getSubtotal() const {
        return static_cast<std::int64_t>(qty) * price;
    }
};

#endif
