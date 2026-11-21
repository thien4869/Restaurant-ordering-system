# Quy ước chung – Group 11

Bám theo Use-case Diagram, Class Diagram, ERD, DFD và kiến trúc 3 lớp trong tài liệu đặc tả. Mọi thành viên đọc file này trước khi code.

## A. Các điểm lệch giữa các sơ đồ – cần chốt ngay

Đây là những chỗ các sơ đồ hoặc đặc tả không khớp nhau. Nếu không chốt, mỗi người sẽ code một kiểu và lúc ghép sẽ lỗi. Cột "Đề xuất" là mặc định, cả nhóm đồng ý thì dùng luôn.

| # | Vấn đề | Đề xuất |
|---|---|---|
| 1 | Đặc tả yêu cầu sắp xếp theo **giảm giá** và tính giá sau giảm, nhưng bảng `PRODUCT` trong ERD và Class Diagram **không có cột discount** | Thêm cột `discount` (số nguyên 0–100, mặc định 0) vào `products` |
| 2 | Tên trạng thái đơn khác nhau: đặc tả ghi New / Confirmed / In Transit / Completed / Cancelled, ảnh giao diện Admin ghi Pending / Preparing / Completed | Dùng bộ của đặc tả (mục B.3). Màn hình Admin hiển thị đúng bộ này |
| 3 | Đặc tả viết "Cancelled" ở chỗ này, "Canceled" ở chỗ khác | Thống nhất `CANCELED` trong CSDL và code |
| 4 | DFD có kho dữ liệu **D6 Payments** và kiến trúc có **Payment Repository**, nhưng ERD **không có bảng payment** (chỉ có `payment_type` trong ORDER). Đặc tả lại yêu cầu "trạng thái thanh toán phải phản ánh kết quả thật" | Không tạo bảng mới. Thêm cột `payment_status` (`PENDING`, `PAID`, `FAILED`) vào `orders`. Phần ghi thanh toán đặt trong `OrderRepository` |
| 5 | ERD có `PROVINCE/DISTRICT/WARD/SHIPPING` và ORDER có `ward_id`, `shipping_id`, nhưng đặc tả use-case không mô tả chọn tỉnh/huyện/xã | Giữ nguyên ERD. TV2 làm form chọn đơn giản ở Checkout, TV5 nhập sẵn vài dòng dữ liệu mẫu (1–2 tỉnh, vài quận, vài phường, 2 kiểu giao hàng) |
| 6 | Use-case Diagram cho **Guest** dùng Manage Cart, nhưng ERD gắn CART với `user_email` | Khách chưa đăng nhập: giỏ lưu trong session. Khi đăng nhập thì gộp vào giỏ trong CSDL. Checkout bắt buộc đăng nhập |
| 7 | Đặc tả và giao diện gọi là **Dish**, sơ đồ lớp và ERD gọi là **Product** | Code và CSDL dùng `product`. Giao diện hiển thị chữ "Dish" như đặc tả |
| 8 | Ảnh dashboard Admin có thêm Tables, Reports, Settings, Dashboard thống kê | **Không làm**. Chỉ làm Menu, Orders, Customers (Users) theo use-case |

## B. Cơ sở dữ liệu

### B.1 Tên bảng

Dùng chữ thường, snake_case, số nhiều. Không đặt `order`, `user` vì trùng từ khóa SQL.

| ERD | Tên bảng | Lớp trong Class Diagram |
|---|---|---|
| USER | `users` | User |
| ADMINISTRATOR | `administrators` | Administrator |
| CATEGORY | `categories` | Category |
| PRODUCT | `products` | Product |
| CART | `carts` | Cart |
| CART_ITEM | `cart_items` | Cart (qty) |
| ORDER | `orders` | Order |
| ORDER_DETAIL | `order_details` | OrderDetail |
| SHIPPING | `shippings` | Shipping |
| PROVINCE / DISTRICT / WARD | `provinces`, `districts`, `wards` | Province, District, Ward |

### B.2 Tên cột

Giữ đúng tên trong ERD, viết snake_case, chỉ đổi những chỗ trùng từ khóa SQL:

| ERD | Dùng |
|---|---|
| `desc` | `description` |
| `view` | `views` |
| `qty` | giữ `qty` |
| `price` (OrderDetail) | giữ `price` (giá lúc đặt, không đổi khi món đổi giá) |

Các khóa chính, khóa ngoại giữ như ERD: `users.email`, `products.code`, `orders.order_number`, `order_details(order_number, product_code)`, `cart_items(cart_id, product_code)`.

Bổ sung theo mục A: `products.discount`, `orders.payment_status`.

### B.3 Giá trị cố định

| Cột | Giá trị lưu trong CSDL | Hiển thị |
|---|---|---|
| `products.status` | `AVAILABLE`, `SOLD_OUT`, `DISCONTINUED` | In stock, Sold Out, (ẩn khỏi menu) |
| `orders.status` | `NEW`, `CONFIRMED`, `IN_TRANSIT`, `COMPLETED`, `CANCELED` | New, Confirmed, In Transit, Completed, Canceled |
| `orders.payment_type` | `CASH`, `ONLINE` | Cash, Online |
| `orders.payment_status` | `PENDING`, `PAID`, `FAILED` | Pending, Paid, Failed |
| `users` / session | vai trò `GUEST`, `CUSTOMER`, `ADMIN` | |

### B.4 Quy tắc dữ liệu

- Mật khẩu **luôn lưu dạng băm** (hash), không lưu chữ thường. Cột vẫn tên `password`.
- Tiền VND, kiểu `DECIMAL(12,0)`. Không dùng số thực.
- Giá sau giảm = `price × (1 − discount/100)`. **Chỉ viết một hàm duy nhất** cho việc này (TV2 phụ trách), mọi nơi gọi lại hàm đó.
- `order_number` theo dạng `ORD-YYYYMMDD-NNNN`, ví dụ `ORD-20261121-0001`.
- Xóa món đã có trong đơn thì chuyển sang `DISCONTINUED`, không xóa dòng (theo use-case Manage Dish).
- Tổng tiền đơn **tính lại ở server** khi checkout, không tin số tiền từ trình duyệt.
- Câu SQL chỉ nằm trong lớp Repository, dùng câu lệnh tham số hóa (chống SQL Injection).

## C. Cấu trúc mã nguồn (theo kiến trúc 3 lớp)

Mỗi lớp chỉ gọi lớp ngay bên dưới. **Giao diện không được gọi thẳng Repository.**

| Lớp | Gồm | Ghi chú |
|---|---|---|
| 1. Presentation | Customer Web (Menu, Cart, Order History, Details, Cancel, Place Order); Admin Dashboard (Order list, search, filter, details, update status) | Chỉ hiển thị và nhận dữ liệu nhập, không chứa nghiệp vụ |
| 2. Business Logic | `AccountService` (login, profile, permissions); `MenuCartService` (browse, manage cart); `OrderService` (place, view, cancel, manage) | Kiểm tra quy tắc nghiệp vụ ở đây |
| 3. Data Access | `UserRepository`, `MenuCartRepository`, `OrderRepository` (kèm thanh toán) | Chỉ ở đây mới có SQL |

Thư mục gợi ý, đặt tên theo ngôn ngữ nhóm chọn:

```
src/
├── presentation/   # trang, controller
│   ├── customer/
│   └── admin/
├── services/
├── repositories/
├── models/         # User, Product, Category, Cart, Order, OrderDetail, Shipping, Administrator
└── common/         # hàm dùng chung: tính giá sau giảm, kiểm tra dữ liệu, session
```

Phân công thư mục: mỗi người chỉ sửa file của module mình. File dùng chung (`common/`, layout, `models/`) muốn sửa thì báo cả nhóm và tạo Pull Request riêng.

### Tên lớp và hàm

- Tên lớp: PascalCase, đúng tên trong Class Diagram (`User`, `Product`, `OrderDetail`...).
- Tên hàm: camelCase, đúng tên trong Class Diagram: `login`, `register`, `updateProfile`, `resetPassword`, `addToCart`, `updateCart`, `removeFromCart`, `checkout`, `cancelOrder`, `viewOrderHistory`, `getDetail`, `manageDishes`, `manageUsers`, `manageOrders`.
- Biến, tên file: theo thông lệ của ngôn ngữ đã chọn (Python: snake_case; Java: camelCase; PHP: camelCase cho hàm).

## D. Giao diện và thông báo

- Chữ trên giao diện và thông báo lỗi dùng **tiếng Anh đúng như trong đặc tả**, ví dụ `Invalid username or password.`, `No matching dish found`, `There are no dishes in this category yet`, `You have no orders yet`. Làm vậy để "Expected Result" trong test case khớp với phần mềm.
- Mỗi trang phân trang 20 dòng (menu, kết quả tìm, danh sách user, danh sách món admin).
- Màn hình theo đúng Figure 3–18 trong đặc tả. Không thêm trang ngoài phạm vi.

## E. Git

- **Không push thẳng vào `main`.** Mỗi người một nhánh: `feature-menu` (TV1), `feature-cart-checkout` (TV2), `feature-account` (TV3), `feature-orders` (TV4), `feature-users` (TV5).
- Xong một chức năng thì tạo Pull Request vào `main`, nhờ một bạn khác xem qua rồi mới gộp. TV5 gộp chốt.
- Commit nhỏ, thường xuyên (đề yêu cầu có bằng chứng dùng công cụ). Cú pháp:

  `[TV3] feat: thêm đăng nhập` · `[TV2] fix: sửa tính tổng giỏ` · `[TV1] test: thêm unit test tìm kiếm`

  Loại: `feat`, `fix`, `test`, `docs`, `refactor`.
- Trước khi bắt đầu làm: `git pull origin main`. Trước khi tạo PR: kéo `main` về nhánh mình và chạy thử.
- Không commit mật khẩu, file cấu hình riêng của máy, thư mục thư viện tải về.

## F. Kiểm thử

### Mã test case

Dùng cột đúng như file mẫu: Test Case ID, Function, Test Steps, Input Data, Expected Result. Để không trùng ID giữa 5 người, ID có tiền tố theo module:

| Module | Tiền tố | Ví dụ |
|---|---|---|
| Thực đơn (TV1) | `MENU` | `TC-MENU-01` |
| Giỏ (TV2) | `CART` | `TC-CART-01` |
| Đặt món (TV2) | `CHK` | `TC-CHK-01` |
| Tài khoản (TV3) | `ACC` | `TC-ACC-01` |
| Đơn của khách (TV4) | `ORD` | `TC-ORD-01` |
| Quản lý đơn của admin (TV4) | `ADM` | `TC-ADM-01` |
| Quản lý người dùng (TV5) | `USR` | `TC-USR-01` |

- Cột **Function** ghi đúng tên use-case trong bảng 1.2 của đặc tả (`Login`, `Register`, `Manage Cart`, `Cancel Order`...).
- Mỗi use-case có ít nhất: 1 trường hợp thành công, 1–2 trường hợp lỗi theo "Alternative flows" của đặc tả, 1 trường hợp biên (số lượng 0, giảm giá 101%, giá âm, từ khóa rỗng, hủy đơn đã Confirmed).
- Mỗi người điền file riêng `TestCases_<Module>.xlsx`, TV5 gộp thành một file nộp.

### Unit test

- Đặt trong `tests/unit/`, tên file theo module: `test_menu`, `test_cart`, `test_checkout`, `test_account`, `test_orders`, `test_admin_orders`, `test_users` (viết theo thông lệ của công cụ test nhóm chọn).
- Ưu tiên test các hàm nghiệp vụ trong Service, và hàm tính giá sau giảm.

### Báo lỗi

Mỗi lỗi là một Issue trên GitHub:

- Tiêu đề: `[BUG][Module] mô tả ngắn`, ví dụ `[BUG][Cart] số lượng 0 vẫn được chấp nhận`.
- Nội dung: các bước tái hiện, kết quả mong đợi, kết quả thực tế, mã test case liên quan (nếu có).
- Nhãn: `bug`, kèm người sửa. Sửa xong thì đóng Issue và ghi số Issue vào commit.

## G. Một chức năng coi là xong khi

1. Code chạy đúng theo use-case, đã gộp vào `main`.
2. Có unit test và đã chạy qua.
3. Có test case trong file Excel, đã chạy thử bằng tay và ghi kết quả.
4. Lỗi tìm được đã ghi Issue, lỗi nặng đã sửa.
5. Có ảnh chụp bằng chứng (commit, Issue) trong `evidence/`.
