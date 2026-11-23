"""
Model Thông tin giao hàng & Phí vận chuyển (src/models/Shipping.py)
"""
from dataclasses import dataclass
from enum import Enum


class DeliveryType(str, Enum):
    STANDARD = "STANDARD"   # Giao hàng tiêu chuẩn
    EXPRESS = "EXPRESS"     # Giao hàng hỏa tốc
    PICKUP = "PICKUP"       # Nhận tại quán


@dataclass
class ShippingInfo:
    recipient_name: str
    phone: str
    province: str
    district: str
    ward: str
    street_address: str
    delivery_type: DeliveryType = DeliveryType.STANDARD
    note: str = ""

    def validate(self) -> None:
        if not self.recipient_name or not self.recipient_name.strip():
            raise ValueError("Họ tên người nhận không được để trống.")
        digits = "".join(ch for ch in (self.phone or "") if ch.isdigit())
        if len(digits) < 9 or len(digits) > 11 or not digits.startswith("0"):
            raise ValueError("Số điện thoại không hợp lệ (phải bắt đầu bằng 0 và dài 9-11 số).")
        if self.delivery_type != DeliveryType.PICKUP:
            if not self.province or not self.province.strip():
                raise ValueError("Vui lòng chọn Tỉnh/Thành phố.")
            if not self.district or not self.district.strip():
                raise ValueError("Vui lòng chọn Quận/Huyện.")
            if not self.ward or not self.ward.strip():
                raise ValueError("Vui lòng chọn Phường/Xã.")
            if not self.street_address or not self.street_address.strip():
                raise ValueError("Vui lòng nhập địa chỉ cụ thể (số nhà, tên đường).")

    def calculate_shipping_fee(self, subtotal: float = 0.0) -> float:
        """
        Tính phí giao hàng theo kiểu giao hàng & khu vực:
        - PICKUP: 0đ
        - STANDARD: Nội thành (HCM/Hà Nội) 15.000đ, tỉnh khác 30.000đ (Miễn phí nếu đơn >= 300.000đ)
        - EXPRESS: Nội thành 30.000đ, tỉnh khác 50.000đ
        """
        if self.delivery_type == DeliveryType.PICKUP:
            return 0.0

        is_inner_city = self.province.strip().lower() in {
            "hồ chí minh", "tp. hồ chí minh", "tp.hcm", "hcm",
            "hà nội", "tp. hà nội", "hn"
        }

        if self.delivery_type == DeliveryType.STANDARD:
            if subtotal >= 300000:
                return 0.0
            return 15000.0 if is_inner_city else 30000.0

        if self.delivery_type == DeliveryType.EXPRESS:
            return 30000.0 if is_inner_city else 50000.0

        raise ValueError("Kiểu giao hàng không hợp lệ.")
