"""
Unit Test cho CheckoutService & CheckoutRepository (tests/Unit/test_checkout.py)
"""
import unittest
from src.repositories.CartRepository import CartRepository
from src.repositories.CheckoutRepository import (
    CheckoutRepository,
    OrderStatus,
    PaymentMethod,
    PaymentStatus,
)
from src.services.CartService import CartService
from src.services.CheckoutService import CheckoutService


class TestCheckoutFlow(unittest.TestCase):
    def setUp(self):
        self.session = {}
        self.cart_repo = CartRepository(session_store=self.session)
        self.checkout_repo = CheckoutRepository()
        self.cart_service = CartService(self.cart_repo)

        # Giả lập catalog trên server để kiểm tra tính lại tổng tiền ở server
        self.server_catalog = {
            "P01": {"product_name": "Phở Bò Đặc Biệt", "unit_price": 70000, "discount_percent": 10},
            "P02": {"product_name": "Trà Sữa Trân Châu", "unit_price": 40000, "discount_percent": 0},
        }
        self.checkout_service = CheckoutService(
            cart_repository=self.cart_repo,
            checkout_repository=self.checkout_repo,
            product_catalog_provider=lambda pid: self.server_catalog.get(pid),
        )

    def test_checkout_cash_order_creates_new_order_and_clears_cart(self):
        self.cart_service.add_item("P01", "Phở Bò Đặc Biệt", 70000, quantity=2, discount_percent=10)
        order = self.checkout_service.place_order(
            recipient_name="Nguyễn Văn A",
            phone="0901234567",
            province="TP. Hồ Chí Minh",
            district="Quận 1",
            ward="Phường Bến Nghé",
            street_address="12 Nguyễn Huệ",
            delivery_type="STANDARD",
            payment_method="CASH",
        )
        self.assertEqual(order.status, OrderStatus.NEW)
        self.assertEqual(order.payment_method, PaymentMethod.CASH)
        self.assertEqual(order.payment_status, PaymentStatus.UNPAID)
        # 70,000 * 0.9 * 2 = 126,000 + 15,000 ship nội thành = 141,000
        self.assertEqual(order.subtotal, 126000.0)
        self.assertEqual(order.shipping_fee, 15000.0)
        self.assertEqual(order.total_amount, 141000.0)
        self.assertEqual(len(self.checkout_repo.get_order_items(order.order_id)), 1)
        # Giỏ hàng phải được làm trống sau khi đặt hàng
        self.assertTrue(self.cart_service.get_cart().is_empty())

    def test_server_recalculates_tampered_client_price(self):
        # Giả sử client lưu giá bị chỉnh sửa thấp hơn (10,000đ, giảm 50%)
        self.cart_service.add_item("P01", "Phở Bò", 10000, quantity=1, discount_percent=50)
        order = self.checkout_service.place_order(
            recipient_name="Trần Thị B",
            phone="0912345678",
            province="TP. Hồ Chí Minh",
            district="Quận 1",
            ward="Phường Bến Thành",
            street_address="45 Lê Lợi",
            delivery_type="PICKUP",
            payment_method="ONLINE",
            simulate_online_success=True,
        )
        # Server phải tính theo giá gốc 70,000 giảm 10% = 63,000đ
        self.assertEqual(order.subtotal, 63000.0)
        self.assertEqual(order.shipping_fee, 0.0)
        self.assertEqual(order.total_amount, 63000.0)
        self.assertEqual(order.payment_status, PaymentStatus.PAID)
        self.assertIsNotNone(order.transaction_id)

    def test_online_payment_failed_sets_failed_status(self):
        self.cart_service.add_item("P02", "Trà Sữa Trân Châu", 40000, quantity=2)
        order = self.checkout_service.place_order(
            recipient_name="Lê Văn C",
            phone="0987654321",
            province="Hà Nội",
            district="Quận Hoàn Kiếm",
            ward="Phường Hàng Bạc",
            street_address="10 Hàng Bạc",
            delivery_type="EXPRESS",
            payment_method="ONLINE",
            simulate_online_success=False,
        )
        self.assertEqual(order.payment_status, PaymentStatus.FAILED)
        self.assertIsNone(order.transaction_id)

    def test_checkout_validation_errors(self):
        # 1. Giỏ hàng trống
        with self.assertRaises(ValueError):
            self.checkout_service.place_order(
                recipient_name="Nguyễn Văn A",
                phone="0901234567",
                province="TP. Hồ Chí Minh",
                district="Quận 1",
                ward="Phường Bến Nghé",
                street_address="12 Nguyễn Huệ",
            )

        # 2. Sai tỉnh/quận/phường hoặc SĐT
        self.cart_service.add_item("P01", "Phở Bò", 70000, quantity=1)
        with self.assertRaises(ValueError):
            self.checkout_service.place_order(
                recipient_name="Nguyễn Văn A",
                phone="12345",  # SĐT sai
                province="TP. Hồ Chí Minh",
                district="Quận 1",
                ward="Phường Bến Nghé",
                street_address="12 Nguyễn Huệ",
            )

        with self.assertRaises(ValueError):
            self.checkout_service.place_order(
                recipient_name="Nguyễn Văn A",
                phone="0901234567",
                province="TP. Hồ Chí Minh",
                district="Quận Không Tồn Tại",
                ward="Phường Bến Nghé",
                street_address="12 Nguyễn Huệ",
            )


if __name__ == "__main__":
    unittest.main()
