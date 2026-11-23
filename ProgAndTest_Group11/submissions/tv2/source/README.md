# TV2 – Giỏ & Đặt món (Nhánh `feature-cart-checkout`)

## 1. Cấu trúc thư mục
- `src/common/PriceHelper.py`: Hàm tính giá sau giảm giá dùng chung (`calculate_discounted_price`, `calculate_line_total`, `calculate_order_total`).
- `src/models/cart.py`: Model `Cart`, `CartItem`.
- `src/models/Shipping.py`: Model `ShippingInfo`, `DeliveryType` & tính phí ship.
- `src/repositories/CartRepository.py`: Lưu giỏ hàng Guest trong session & thành viên.
- `src/repositories/CheckoutRepository.py`: Tạo đơn (`NEW`), lưu chi tiết đơn & cập nhật `payment_status`.
- `src/services/CartService.py`: Nghiệp vụ quản lý giỏ hàng (thêm, sửa số lượng, xóa, tổng tiền).
- `src/services/CheckoutService.py`: Kiểm tra form địa chỉ (Tỉnh/Quận/Phường), kiểu giao hàng, tính lại tổng tiền ở server, thanh toán `CASH`/`ONLINE` giả lập.
- `src/presentation/customer/cart/` & `src/presentation/customer/checkout/`: Giao diện HTML hiển thị giỏ hàng, form checkout và xác nhận đơn.
- `tests/Unit/test_cart.py` & `tests/Unit/test_checkout.py`: Bộ Unit Test đầy đủ.
- `tests/testcases/TestCases_CartCheckout.xlsx`: Bộ Test Case đầy đủ với tiền tố `TC-CART-xx` và `TC-CHK-xx`.

## 2. Hướng dẫn chạy Unit Test
```bash
python -m unittest discover -s tests/Unit -v
```

## 3. Hướng dẫn quy trình Git & GitHub Issues (TV2)
1. Tạo và chuyển sang nhánh làm việc:
   ```bash
   git checkout -b feature-cart-checkout
   ```
2. Commit từng phần chức năng rõ ràng (chụp ảnh màn hình lịch sử commit làm bằng chứng):
   ```bash
   git add src/common/PriceHelper.py src/models/
   git commit -m "feat(tv2): implement PriceHelper, Cart and Shipping models"
   git add src/repositories/ src/services/ src/presentation/
   git commit -m "feat(tv2): implement Cart & Checkout flow with server-side price recalculation"
   git add tests/
   git commit -m "test(tv2): add unit tests and TestCases_CartCheckout.xlsx"
   git push -u origin feature-cart-checkout
   ```
3. Ghi lỗi phát hiện được vào tab **Issues** trên GitHub (ví dụ: `[BUG][TC-CHK-08] Cho phép chọn Phường không thuộc Quận khi chỉnh sửa payload request`), đính kèm bước tái hiện, kết quả mong đợi, kết quả thực tế và ảnh chụp.
