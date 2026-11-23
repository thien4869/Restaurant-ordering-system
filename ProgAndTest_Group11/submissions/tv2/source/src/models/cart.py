"""
Model Giỏ hàng & Chi tiết giỏ hàng (src/models/cart.py)
"""
from dataclasses import dataclass, field
from typing import Dict, List
from src.common.PriceHelper import PriceHelper


@dataclass
class CartItem:
    product_id: str
    product_name: str
    unit_price: float
    quantity: int = 1
    discount_percent: float = 0.0
    note: str = ""

    def __post_init__(self):
        if not self.product_id or not str(self.product_id).strip():
            raise ValueError("Mã sản phẩm không được để trống.")
        if self.unit_price < 0:
            raise ValueError("Đơn giá sản phẩm không hợp lệ.")
        if self.quantity <= 0:
            raise ValueError("Số lượng sản phẩm phải lớn hơn 0.")
        if not (0 <= self.discount_percent <= 100):
            raise ValueError("Phần trăm giảm giá phải từ 0 đến 100.")

    @property
    def discounted_unit_price(self) -> float:
        return PriceHelper.calculate_discounted_price(self.unit_price, self.discount_percent)

    @property
    def subtotal(self) -> float:
        return PriceHelper.calculate_line_total(self.unit_price, self.quantity, self.discount_percent)

    def to_dict(self) -> dict:
        return {
            "product_id": self.product_id,
            "product_name": self.product_name,
            "unit_price": self.unit_price,
            "discount_percent": self.discount_percent,
            "discounted_unit_price": self.discounted_unit_price,
            "quantity": self.quantity,
            "subtotal": self.subtotal,
            "note": self.note,
        }

    @classmethod
    def from_dict(cls, data: dict) -> "CartItem":
        return cls(
            product_id=data["product_id"],
            product_name=data["product_name"],
            unit_price=float(data["unit_price"]),
            quantity=int(data["quantity"]),
            discount_percent=float(data.get("discount_percent", 0.0)),
            note=data.get("note", ""),
        )


@dataclass
class Cart:
    cart_id: str
    user_id: str | None = None
    items: Dict[str, CartItem] = field(default_factory=dict)

    @property
    def total_quantity(self) -> int:
        return sum(item.quantity for item in self.items.values())

    @property
    def total_amount(self) -> float:
        return sum(item.subtotal for item in self.items.values())

    def is_empty(self) -> bool:
        return len(self.items) == 0

    def to_dict(self) -> dict:
        return {
            "cart_id": self.cart_id,
            "user_id": self.user_id,
            "items": {pid: item.to_dict() for pid, item in self.items.items()},
            "total_quantity": self.total_quantity,
            "total_amount": self.total_amount,
        }

    @classmethod
    def from_dict(cls, data: dict) -> "Cart":
        items = {
            pid: CartItem.from_dict(item_data)
            for pid, item_data in data.get("items", {}).items()
        }
        return cls(
            cart_id=data.get("cart_id", "guest_cart"),
            user_id=data.get("user_id"),
            items=items,
        )
