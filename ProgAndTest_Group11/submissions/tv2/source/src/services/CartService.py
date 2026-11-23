"""
Service Xử lý nghiệp vụ Giỏ hàng (src/services/CartService.py)
Manage Cart: thêm, sửa số lượng, xóa, tính tổng tiền; giỏ của Guest lưu trong session.
"""
from typing import Optional
from src.models.cart import Cart, CartItem
from src.repositories.CartRepository import CartRepository


class CartService:
    MAX_ITEM_QUANTITY = 99

    def __init__(self, cart_repository: CartRepository):
        self.cart_repository = cart_repository

    def get_cart(self, user_id: Optional[str] = None) -> Cart:
        return self.cart_repository.get_cart(user_id=user_id)

    def add_item(
        self,
        product_id: str,
        product_name: str,
        unit_price: float,
        quantity: int = 1,
        discount_percent: float = 0.0,
        note: str = "",
        user_id: Optional[str] = None,
    ) -> Cart:
        if quantity <= 0:
            raise ValueError("Số lượng thêm vào giỏ phải lớn hơn 0.")
        if quantity > self.MAX_ITEM_QUANTITY:
            raise ValueError(f"Số lượng tối đa cho mỗi món là {self.MAX_ITEM_QUANTITY}.")

        cart = self.get_cart(user_id=user_id)
        if product_id in cart.items:
            existing = cart.items[product_id]
            new_qty = existing.quantity + quantity
            if new_qty > self.MAX_ITEM_QUANTITY:
                raise ValueError(f"Tổng số lượng món '{product_name}' không được vượt quá {self.MAX_ITEM_QUANTITY}.")
            existing.quantity = new_qty
            existing.unit_price = unit_price
            existing.discount_percent = discount_percent
            if note:
                existing.note = note
        else:
            cart.items[product_id] = CartItem(
                product_id=product_id,
                product_name=product_name,
                unit_price=unit_price,
                quantity=quantity,
                discount_percent=discount_percent,
                note=note,
            )

        return self.cart_repository.save_cart(cart)

    def update_quantity(self, product_id: str, quantity: int, user_id: Optional[str] = None) -> Cart:
        cart = self.get_cart(user_id=user_id)
        if product_id not in cart.items:
            raise KeyError(f"Sản phẩm '{product_id}' không tồn tại trong giỏ hàng.")

        if quantity <= 0:
            # Nếu cập nhật số lượng <= 0 thì xóa sản phẩm khỏi giỏ
            del cart.items[product_id]
        else:
            if quantity > self.MAX_ITEM_QUANTITY:
                raise ValueError(f"Số lượng tối đa cho mỗi món là {self.MAX_ITEM_QUANTITY}.")
            cart.items[product_id].quantity = quantity

        return self.cart_repository.save_cart(cart)

    def remove_item(self, product_id: str, user_id: Optional[str] = None) -> Cart:
        cart = self.get_cart(user_id=user_id)
        if product_id not in cart.items:
            raise KeyError(f"Sản phẩm '{product_id}' không tồn tại trong giỏ hàng.")
        del cart.items[product_id]
        return self.cart_repository.save_cart(cart)

    def clear_cart(self, user_id: Optional[str] = None) -> Cart:
        self.cart_repository.clear_cart(user_id=user_id)
        return self.get_cart(user_id=user_id)

    def get_cart_summary(self, user_id: Optional[str] = None) -> dict:
        cart = self.get_cart(user_id=user_id)
        return {
            "cart_id": cart.cart_id,
            "is_guest": cart.user_id is None,
            "items": [item.to_dict() for item in cart.items.values()],
            "total_quantity": cart.total_quantity,
            "total_amount": cart.total_amount,
        }
