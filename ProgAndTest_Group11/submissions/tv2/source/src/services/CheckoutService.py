"""
Service Xử lý Đặt hàng & Thanh toán (src/services/CheckoutService.py)
- Kiểm tra dữ liệu form checkout (tỉnh/quận/phường, kiểu giao hàng)
- Tính lại tổng tiền ở server (không tin tưởng số tiền client gửi lên)
- Tạo đơn hàng (trạng thái NEW) + chi tiết đơn
- Thanh toán Cash / Online (giả lập) + cập nhật payment_status
"""
import uuid
from typing import Callable, Dict, Optional
from src.common.PriceHelper import PriceHelper
from src.models.Shipping import DeliveryType, ShippingInfo
from src.repositories.CartRepository import CartRepository
from src.repositories.CheckoutRepository import (
    CheckoutRepository,
    OrderItemRecord,
    OrderRecord,
    OrderStatus,
    PaymentMethod,
    PaymentStatus,
)


PROVINCE_DISTRICTS_WARDS = {
    "TP. Hồ Chí Minh": {
        "Quận 1": ["Phường Bến Nghé", "Phường Bến Thành", "Phường Đa Kao"],
        "Quận 3": ["Phường Võ Thị Sáu", "Phường 1", "Phường 2"],
        "TP. Thủ Đức": ["Phường Linh Trung", "Phường Thảo Điền", "Phường Hiệp Bình Chánh"],
    },
    "Hà Nội": {
        "Quận Hoàn Kiếm": ["Phường Hàng Bạc", "Phường Tràng Tiền", "Phường Hàng Gai"],
        "Quận Cầu Giấy": ["Phường Dịch Vọng", "Phường Mai Dịch", "Phường Nghĩa Tân"],
    },
    "Đà Nẵng": {
        "Quận Hải Châu": ["Phường Thạch Thang", "Phường Hải Châu 1"],
    },
}


class CheckoutService:
    def __init__(
        self,
        cart_repository: CartRepository,
        checkout_repository: CheckoutRepository,
        product_catalog_provider: Optional[Callable[[str], Optional[dict]]] = None,
    ):
        """
        product_catalog_provider: hàm trả về thông tin giá gốc/giảm giá mới nhất từ server theo product_id
        (giúp server tính lại tổng tiền chính xác, chống sửa giá từ client).
        """
        self.cart_repository = cart_repository
        self.checkout_repository = checkout_repository
        self.product_catalog_provider = product_catalog_provider

    def validate_location(self, province: str, district: str, ward: str) -> None:
        if province not in PROVINCE_DISTRICTS_WARDS:
            raise ValueError(f"Tỉnh/Thành phố '{province}' không nằm trong danh sách hỗ trợ.")
        districts = PROVINCE_DISTRICTS_WARDS[province]
        if district not in districts:
            raise ValueError(f"Quận/Huyện '{district}' không thuộc '{province}'.")
        wards = districts[district]
        if ward not in wards:
            raise ValueError(f"Phường/Xã '{ward}' không thuộc '{district}'.")

    def place_order(
        self,
        recipient_name: str,
        phone: str,
        province: str,
        district: str,
        ward: str,
        street_address: str,
        delivery_type: str = "STANDARD",
        payment_method: str = "CASH",
        note: str = "",
        user_id: Optional[str] = None,
        simulate_online_success: bool = True,
    ) -> OrderRecord:
        cart = self.cart_repository.get_cart(user_id=user_id)
        if cart.is_empty():
            raise ValueError("Giỏ hàng đang trống, không thể tiến hành đặt món.")

        try:
            d_type = DeliveryType(delivery_type.upper())
        except ValueError:
            raise ValueError("Kiểu giao hàng không hợp lệ (chỉ hỗ trợ STANDARD, EXPRESS, PICKUP).")

        try:
            p_method = PaymentMethod(payment_method.upper())
        except ValueError:
            raise ValueError("Phương thức thanh toán không hợp lệ (chỉ hỗ trợ CASH, ONLINE).")

        shipping = ShippingInfo(
            recipient_name=recipient_name,
            phone=phone,
            province=province,
            district=district,
            ward=ward,
            street_address=street_address,
            delivery_type=d_type,
            note=note,
        )
        shipping.validate()
        if d_type != DeliveryType.PICKUP:
            self.validate_location(province, district, ward)

        # Tính lại tổng tiền ở server (lấy giá mới nhất từ catalog nếu có)
        order_id = self.checkout_repository.generate_order_id()
        order_items: list[OrderItemRecord] = []
        server_subtotal = 0.0

        for idx, item in enumerate(cart.items.values(), start=1):
            unit_price = item.unit_price
            discount_percent = item.discount_percent
            product_name = item.product_name

            if self.product_catalog_provider:
                catalog_item = self.product_catalog_provider(item.product_id)
                if not catalog_item:
                    raise ValueError(f"Món '{item.product_name}' hiện không còn phục vụ.")
                unit_price = float(catalog_item["unit_price"])
                discount_percent = float(catalog_item.get("discount_percent", 0.0))
                product_name = catalog_item.get("product_name", product_name)

            final_unit = PriceHelper.calculate_discounted_price(unit_price, discount_percent)
            line_total = PriceHelper.calculate_line_total(unit_price, item.quantity, discount_percent)
            server_subtotal += line_total

            order_items.append(
                OrderItemRecord(
                    order_item_id=f"{order_id}-ITEM-{idx:02d}",
                    order_id=order_id,
                    product_id=item.product_id,
                    product_name=product_name,
                    unit_price=unit_price,
                    discount_percent=discount_percent,
                    final_unit_price=final_unit,
                    quantity=item.quantity,
                    line_total=line_total,
                )
            )

        shipping_fee = shipping.calculate_shipping_fee(subtotal=server_subtotal)
        server_total = PriceHelper.calculate_order_total(subtotal=server_subtotal, shipping_fee=shipping_fee)

        full_address = (
            "Nhận tại cửa hàng"
            if d_type == DeliveryType.PICKUP
            else f"{street_address}, {ward}, {district}, {province}"
        )

        order = OrderRecord(
            order_id=order_id,
            user_id=user_id,
            recipient_name=recipient_name.strip(),
            phone=phone.strip(),
            full_address=full_address,
            delivery_type=d_type.value,
            subtotal=server_subtotal,
            shipping_fee=shipping_fee,
            total_amount=server_total,
            status=OrderStatus.NEW,
            payment_method=p_method,
            payment_status=PaymentStatus.UNPAID,
            note=note,
        )

        self.checkout_repository.create_order(order, order_items)

        # Xử lý thanh toán Cash / Online (giả lập)
        self.process_payment(order_id=order.order_id, simulate_online_success=simulate_online_success)

        # Xóa giỏ hàng sau khi tạo đơn thành công
        self.cart_repository.clear_cart(user_id=user_id)

        return self.checkout_repository.get_order_by_id(order_id)

    def process_payment(self, order_id: str, simulate_online_success: bool = True) -> OrderRecord:
        order = self.checkout_repository.get_order_by_id(order_id)
        if not order:
            raise KeyError(f"Không tìm thấy đơn hàng: {order_id}")

        if order.payment_method == PaymentMethod.CASH:
            return self.checkout_repository.update_payment_status(
                order_id=order_id,
                payment_status=PaymentStatus.UNPAID,
                transaction_id=None,
            )

        # Thanh toán Online (giả lập cổng thanh toán)
        if simulate_online_success:
            tx_id = f"PAY-{uuid.uuid4().hex[:10].upper()}"
            return self.checkout_repository.update_payment_status(
                order_id=order_id,
                payment_status=PaymentStatus.PAID,
                transaction_id=tx_id,
            )
        else:
            return self.checkout_repository.update_payment_status(
                order_id=order_id,
                payment_status=PaymentStatus.FAILED,
                transaction_id=None,
            )
