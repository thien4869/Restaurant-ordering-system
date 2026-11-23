"""
Unit Test cho PriceHelper & CartService (tests/Unit/test_cart.py)
"""
import unittest
from src.common.PriceHelper import PriceHelper
from src.repositories.CartRepository import CartRepository
from src.services.CartService import CartService


class TestPriceHelperAndCart(unittest.TestCase):
    def setUp(self):
        self.session = {}
        self.cart_repo = CartRepository(session_store=self.session)
        self.cart_service = CartService(self.cart_repo)

    # --- Test PriceHelper ---
    def test_calculate_discounted_price_valid(self):
        self.assertEqual(PriceHelper.calculate_discounted_price(100000, 10), 90000.0)
        self.assertEqual(PriceHelper.calculate_discounted_price(50000, 0), 50000.0)
        self.assertEqual(PriceHelper.calculate_discounted_price(50000, 100), 0.0)

    def test_calculate_discounted_price_invalid(self):
        with self.assertRaises(ValueError):
            PriceHelper.calculate_discounted_price(-1000, 10)
        with self.assertRaises(ValueError):
            PriceHelper.calculate_discounted_price(50000, -5)
        with self.assertRaises(ValueError):
            PriceHelper.calculate_discounted_price(50000, 105)

    # --- Test Manage Cart (Guest Session) ---
    def test_guest_cart_stored_in_session(self):
        self.cart_service.add_item("P01", "Phở Bò", 60000, quantity=2, discount_percent=10)
        self.assertIn(CartRepository.SESSION_CART_KEY, self.session)
        summary = self.cart_service.get_cart_summary()
        self.assertTrue(summary["is_guest"])
        self.assertEqual(summary["total_quantity"], 2)
        self.assertEqual(summary["total_amount"], 108000.0)

    def test_add_duplicate_item_merges_quantity(self):
        self.cart_service.add_item("P01", "Phở Bò", 60000, quantity=1)
        self.cart_service.add_item("P01", "Phở Bò", 60000, quantity=3)
        cart = self.cart_service.get_cart()
        self.assertEqual(cart.items["P01"].quantity, 4)
        self.assertEqual(cart.total_amount, 240000.0)

    def test_update_quantity_and_auto_remove_when_zero(self):
        self.cart_service.add_item("P01", "Cơm Tấm", 45000, quantity=2)
        self.cart_service.update_quantity("P01", 5)
        self.assertEqual(self.cart_service.get_cart().items["P01"].quantity, 5)
        self.assertEqual(self.cart_service.get_cart().total_amount, 225000.0)

        # Cập nhật = 0 -> tự xóa khỏi giỏ
        self.cart_service.update_quantity("P01", 0)
        self.assertTrue(self.cart_service.get_cart().is_empty())

    def test_remove_item_success_and_not_found(self):
        self.cart_service.add_item("P01", "Bún Chả", 55000, quantity=1)
        self.cart_service.remove_item("P01")
        self.assertTrue(self.cart_service.get_cart().is_empty())

        with self.assertRaises(KeyError):
            self.cart_service.remove_item("P99")

    def test_add_item_invalid_quantity(self):
        with self.assertRaises(ValueError):
            self.cart_service.add_item("P01", "Trà Đào", 30000, quantity=0)
        with self.assertRaises(ValueError):
            self.cart_service.add_item("P01", "Trà Đào", 30000, quantity=100)


if __name__ == "__main__":
    unittest.main()
