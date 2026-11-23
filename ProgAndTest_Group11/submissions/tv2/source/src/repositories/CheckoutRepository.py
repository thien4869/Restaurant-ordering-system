"""
Repository Đặt hàng & Thanh toán (src/repositories/CheckoutRepository.py)
Quản lý: tạo đơn (trạng thái NEW), chi tiết đơn, cập nhật trạng thái thanh toán (payment_status).
"""
from dataclasses import dataclass, field
from datetime import datetime
from enum import Enum
from typing import Dict, List, Optional
import uuid


class OrderStatus(str, Enum):
    NEW = "NEW"
    CONFIRMED = "CONFIRMED"
    CANCELLED = "CANCELLED"


class PaymentMethod(str, Enum):
    CASH = "CASH"
    ONLINE = "ONLINE"


class PaymentStatus(str, Enum):
    UNPAID = "UNPAID"
    PENDING = "PENDING"
    PAID = "PAID"
    FAILED = "FAILED"


@dataclass
class OrderItemRecord:
    order_item_id: str
    order_id: str
    product_id: str
    product_name: str
    unit_price: float
    discount_percent: float
    final_unit_price: float
    quantity: int
    line_total: float


@dataclass
class OrderRecord:
    order_id: str
    user_id: Optional[str]
    recipient_name: str
    phone: str
    full_address: str
    delivery_type: str
    subtotal: float
    shipping_fee: float
    total_amount: float
    status: OrderStatus = OrderStatus.NEW
    payment_method: PaymentMethod = PaymentMethod.CASH
    payment_status: PaymentStatus = PaymentStatus.UNPAID
    transaction_id: Optional[str] = None
    note: str = ""
    created_at: str = field(default_factory=lambda: datetime.now().isoformat())
    items: List[OrderItemRecord] = field(default_factory=list)


class CheckoutRepository:
    def __init__(self):
        self._orders: Dict[str, OrderRecord] = {}
        self._order_items: Dict[str, List[OrderItemRecord]] = {}

    def generate_order_id(self) -> str:
        return f"ORD-{uuid.uuid4().hex[:8].upper()}"

    def create_order(self, order: OrderRecord, items: List[OrderItemRecord]) -> OrderRecord:
        """Lưu đơn hàng (trạng thái NEW) cùng danh sách chi tiết đơn."""
        order.status = OrderStatus.NEW
        order.items = list(items)
        self._orders[order.order_id] = order
        self._order_items[order.order_id] = list(items)
        return order

    def get_order_by_id(self, order_id: str) -> Optional[OrderRecord]:
        return self._orders.get(order_id)

    def get_order_items(self, order_id: str) -> List[OrderItemRecord]:
        return list(self._order_items.get(order_id, []))

    def update_payment_status(
        self,
        order_id: str,
        payment_status: PaymentStatus,
        transaction_id: Optional[str] = None,
    ) -> OrderRecord:
        order = self.get_order_by_id(order_id)
        if not order:
            raise KeyError(f"Không tìm thấy đơn hàng: {order_id}")
        order.payment_status = payment_status
        if transaction_id:
            order.transaction_id = transaction_id
        return order

    def list_all_orders(self) -> List[OrderRecord]:
        return list(self._orders.values())
