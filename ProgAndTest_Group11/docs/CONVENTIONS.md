# Quy ước chung – Group 11 (C++ / OOP)

*Cập nhật 23/11/2026. Áp dụng cho hướng triển khai C++ mới; giữ nguyên tài liệu đặc tả, database và lịch sử nộp bài. Những quyết định chưa thống nhất được đánh dấu **CẦN CHỐT**.*

## A. Mục tiêu, ranh giới và các quyết định cần chốt

- Chương trình: **Restaurant Order Management System**, ngôn ngữ C++17 (đề xuất), sử dụng OOP và tách ba lớp.
- **CẦN CHỐT:** GUI nào (Qt, thư viện khác, hoặc xác nhận giáo viên chấp nhận console prototype); Stage 2 yêu cầu GUI design.
- **CẦN CHỐT:** cách lưu dữ liệu (MySQL + C++ connector, SQLite, file) và mô hình triển khai Docker. `docker-compose.yml` hiện tại dành cho PHP/MySQL, **chưa chạy C++**.
- Các ZIP đã nộp được lưu ở `submissions/` để bảo toàn bản gốc. Không biên dịch/gộp bừa tất cả mã C++ trong ZIP thành một chương trình.
- Một executable chỉ có **một `main()`**. Các `main.cpp` demo của thành viên cần tách/chuyển đổi khi tích hợp.
- Chức năng Account TV3 phải tích hợp với cùng nguồn dữ liệu User của TV5; Orders TV4/Checkout TV2 dùng chung các kiểu Order, Product, User.

## B. Hợp đồng dữ liệu dựa trên đặc tả hiện có

Giữ tên nghiệp vụ và quan hệ trong ERD/Use-case. Nếu dùng SQL, tên bảng: `users`, `administrators`, `categories`, `products`, `carts`, `cart_items`, `orders`, `order_details`, `shippings`, `provinces`, `districts`, `wards`. Khóa chính `users.email`, `products.code`, `orders.order_number`.

| Nội dung | Quy tắc thống nhất |
|---|---|
| Product | `discount` số nguyên 0–100, `AVAILABLE` / `SOLD_OUT` / `DISCONTINUED` |
| Đơn hàng | `NEW`, `CONFIRMED`, `IN_TRANSIT`, `COMPLETED`, `CANCELED` |
| Thanh toán | `payment_type`: `CASH` / `ONLINE`; `payment_status`: `PENDING` / `PAID` / `FAILED` |
| Vai trò | `GUEST`, `CUSTOMER`, `ADMIN` |
| Mã đơn | `ORD-YYYYMMDD-NNNN` (theo đặc tả hiện tại) |
| Giá | VND nguyên; tính tiền bằng kiểu số nguyên 64-bit hoặc decimal, **không dùng floating-point** cho tiền |

- Giữ tên `Product` trong code dù giao diện có thể gọi là “Dish”. Tiền sau giảm và tổng checkout cần tính ở lớp Service, không tin dữ liệu UI.
- Hủy đơn chỉ khi `NEW`; món đã có trong đơn chuyển `DISCONTINUED` thay vì xóa vật lý.
- Không lưu mật khẩu dạng plaintext hoặc dùng `std::hash` để bảo vệ mật khẩu. Cần thư viện password hashing phù hợp và quy trình xác thực/reset an toàn; module C++ demo hiện chưa đáp ứng.
- Nếu dùng SQL, **chỉ Repository truy vấn DB** và phải dùng parameter binding/prepared statement. Nếu dùng file/in-memory, interface Repository vẫn cần tách khỏi Service.
- Giữ nguyên `database/schema.sql` và `seed.sql` như tài liệu lịch sử đến khi quyết định persistence được xác nhận.

## C. Kiến trúc và cấu trúc C++

```text
src/
├── main.cpp
├── models/              # User.h, Product.h, Order.h, ...
├── repositories/        # UserRepository.h/.cpp, OrderRepository.h/.cpp, ...
├── services/            # AccountService.h/.cpp, OrderService.h/.cpp, ...
├── presentation/        # giao diện chung, Customer/Admin menu hoặc GUI
└── common/              # validators, config, helpers chia sẻ
tests/
├── unit/                # Google Test hoặc bộ test C++ phù hợp
├── integration/
└── testcases/           # Excel
submissions/             # lưu ZIP gốc chưa tích hợp
evidence/                # kết quả test, ảnh, issue/PR
```

- **Presentation → Service → Repository**. UI không gọi thẳng Repository; Repository không gọi UI.
- `models/`: khai báo dữ liệu dùng chung, không tái định nghĩa `User`/`Order` khác nhau ở các module.
- Dùng `.h` (hoặc `.hpp` nếu chốt chung) cho interface, `.cpp` cho implementation; include guard/`#pragma once`.
- Tên class `PascalCase` (`User`, `OrderService`); hàm/biến `camelCase` (`getUserByEmail`, `cancelOrder`); file trùng tên class hoặc thống nhất theo module. `const` đúng chỗ, ưu tiên `std::vector`, `std::string`, smart pointer và RAII.
- Dữ liệu nhạy cảm không log ra console, không commit secret/executable/build output.
- Thư viện build chính được đề xuất là CMake; chưa coi là hoàn tất cho đến khi có `CMakeLists.txt` build được.

## D. Giao diện và tính năng

- Text của giao diện và lỗi theo **tiếng Anh** như đặc tả để đối chiếu Expected Result.
- Các danh sách/tìm kiếm theo quy ước **20 dòng/trang**.
- Phạm vi admin: quản lý Menu, Orders và Users; không tự thêm báo cáo/dashboard ngoài use-case.
- Console app trong ZIP có thể dùng kiểm chứng nghiệp vụ, **không chứng minh đã hoàn thành GUI**. Trước khi nộp, phải xác nhận yêu cầu Stage 2 và thể hiện thiết kế GUI.

## E. Phân công, nhánh Git và nộp file

| TV | Module | Nhánh đề xuất |
|---|---|---|
| TV1 | Menu | `feature-menu` |
| TV2 | Cart/Checkout | `feature-cart-checkout` |
| TV3 | Account | `feature-account` |
| TV4 | Orders | `feature-orders` |
| TV5 | Manage Users & Integration | `feature-users` |

- Không push trực tiếp `main`; nộp từng module qua Pull Request, review rồi merge.
- Commit `[TV3] feat: implement login`, `[TV5] test: add users tests`. Loại: `feat`, `fix`, `test`, `docs`, `refactor`.
- Các ZIP gốc không đặt trong `tests/`; lưu `submissions/tvN/` để truy xuất. **Mã đã tích hợp** đặt trong `src/`, unit test thật trong `tests/unit/`, Excel trong `tests/testcases/`.
- File cùng nội dung hoặc cùng tên cần so sánh trước khi xóa. Không ghi đè workbook đã nộp; giữ cả hai phiên bản đến khi TV5 xác nhận.
- Cần PR riêng trước khi sửa code dùng chung ở `models/`, `common/`, build system.

## F. Testing và quản lý lỗi

| Module | Test Case ID |
|---|---|
| Menu (TV1) | `TC-MENU-01` |
| Cart (TV2) | `TC-CART-01` |
| Checkout (TV2) | `TC-CHK-01` |
| Account (TV3) | `TC-ACC-01` |
| Customer Orders (TV4) | `TC-ORD-01` |
| Admin Orders (TV4) | `TC-ADM-01` |
| Manage Users (TV5) | `TC-USR-01` |

- Excel tối thiểu có **Test Case ID, Function, Test Steps, Input Data, Expected Result**; khi chạy bổ sung **Actual Result, Status, Evidence**.
- Mỗi use-case: ca thành công, ca lỗi, ca biên. Không đánh PASS nếu chưa chạy với code thực tế.
- Unit test ưu tiên **Google Test (C++)** cho lớp Service (mock/fake repository). Test chạm DB thật được ghi rõ là integration test.
- Mỗi lỗi ghi GitHub Issue: `[BUG][Module] mô tả`, bước tái hiện, expected, actual, test ID, bằng chứng và người phụ trách.
- Chỉ coi module hoàn thành khi: code build/run chung; có test chạy được; Excel ghi actual/status; lỗi nghiêm trọng được xử lý; có minh chứng trong `evidence/`.

## G. Ghi chú chuyển đổi từ PHP

- Các file PHP/Apache/Docker hiện có là phiên bản thử nghiệm trước khi chốt C++; không dùng chúng làm bằng chứng C++ chạy được.
- **Không xóa** đặc tả/sơ đồ/bảng phân công hoặc dữ liệu thành viên đã nộp. Mọi chuyển đổi cần giữ lịch sử Git và được review.
- Kiểm tra các bài test/demo C++ hiện có, chuyển từng module sang interface dùng chung rồi mới xóa code trùng.
