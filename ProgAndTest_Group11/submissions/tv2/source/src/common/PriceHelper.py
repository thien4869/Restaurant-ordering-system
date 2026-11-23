"""
Hàm tính giá sau giảm giá dùng chung (src/common/PriceHelper.py)
Dùng chung cho toàn bộ hệ thống (Menu, Cart, Checkout).
"""
from decimal import Decimal, ROUND_HALF_UP


class PriceHelper:
    @staticmethod
    def calculate_discounted_price(unit_price: float, discount_percent: float = 0.0) -> float:
        """
        Tính đơn giá sau khi áp dụng phần trăm giảm giá.
        - unit_price: giá gốc (>= 0)
        - discount_percent: % giảm giá (0 -> 100)
        Trả về giá đã làm tròn chuẩn (VND: làm tròn đến hàng đơn vị).
        """
        if unit_price < 0:
            raise ValueError("Đơn giá không được âm.")
        if discount_percent < 0 or discount_percent > 100:
            raise ValueError("Phần trăm giảm giá phải nằm trong khoảng 0 đến 100.")

        price = Decimal(str(unit_price))
        discount = Decimal(str(discount_percent))
        final_price = price * (Decimal("100") - discount) / Decimal("100")
        return float(final_price.quantize(Decimal("1"), rounding=ROUND_HALF_UP))

    @staticmethod
    def calculate_line_total(unit_price: float, quantity: int, discount_percent: float = 0.0) -> float:
        """Tính thành tiền cho 1 dòng sản phẩm sau giảm giá."""
        if quantity <= 0:
            raise ValueError("Số lượng sản phẩm phải lớn hơn 0.")
        discounted_unit = PriceHelper.calculate_discounted_price(unit_price, discount_percent)
        return float(Decimal(str(discounted_unit)) * Decimal(str(quantity)))

    @staticmethod
    def calculate_order_total(subtotal: float, shipping_fee: float = 0.0, voucher_discount: float = 0.0) -> float:
        """
        Tính tổng thanh toán cuối cùng của đơn hàng:
        total = max(0, subtotal - voucher_discount) + shipping_fee
        """
        if subtotal < 0 or shipping_fee < 0 or voucher_discount < 0:
            raise ValueError("Giá trị tiền không được âm.")
        after_voucher = max(0.0, subtotal - voucher_discount)
        return float(Decimal(str(after_voucher)) + Decimal(str(shipping_fee)))
