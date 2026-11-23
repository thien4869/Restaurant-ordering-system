"""
Repository Giỏ hàng (src/repositories/CartRepository.py)
Hỗ trợ lưu trữ giỏ hàng của Guest trong Session và giỏ hàng của thành viên đã đăng nhập.
"""
from typing import Dict, Optional
from src.models.cart import Cart


class CartRepository:
    SESSION_CART_KEY = "guest_cart"

    def __init__(self, session_store: Optional[dict] = None):
        """
        - session_store: dict đại diện cho session hiện tại (dành cho Guest).
        - _user_carts: kho lưu trữ theo user_id (mô phỏng DB cho user đã đăng nhập).
        """
        self.session_store: dict = session_store if session_store is not None else {}
        self._user_carts: Dict[str, dict] = {}

    def get_cart(self, user_id: Optional[str] = None) -> Cart:
        if user_id:
            raw = self._user_carts.get(user_id)
            if not raw:
                cart = Cart(cart_id=f"cart_{user_id}", user_id=user_id)
                self.save_cart(cart)
                return cart
            return Cart.from_dict(raw)

        # Guest -> lưu trong session
        raw_session = self.session_store.get(self.SESSION_CART_KEY)
        if not raw_session:
            cart = Cart(cart_id="guest_session_cart", user_id=None)
            self.save_cart(cart)
            return cart
        return Cart.from_dict(raw_session)

    def save_cart(self, cart: Cart) -> Cart:
        serialized = cart.to_dict()
        if cart.user_id:
            self._user_carts[cart.user_id] = serialized
        else:
            self.session_store[self.SESSION_CART_KEY] = serialized
        return cart

    def clear_cart(self, user_id: Optional[str] = None) -> None:
        empty_cart = Cart(
            cart_id=f"cart_{user_id}" if user_id else "guest_session_cart",
            user_id=user_id,
        )
        self.save_cart(empty_cart)
