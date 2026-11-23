# Restaurant Order Management System – Group 11

Đồ án nhóm: phân tích, thiết kế, cài đặt và kiểm thử hệ thống quản lý đặt món nhà hàng.

> **Hướng triển khai mới (23/11/2026): C++ hướng đối tượng (OOP)**. Tài liệu này mô tả **cấu trúc mục tiêu**, không khẳng định rằng tất cả module đã tích hợp/chạy được. Mã nguồn C++ của các thành viên đang được rà soát; tuyệt đối không xem file ZIP trong `submissions/` là ứng dụng hoàn chỉnh.

## Phạm vi và phân công

| Thành viên | Module | Use-case chính |
|---|---|---|
| TV1 | Menu / Product / Category | Browse Menu, Search Dish, View Dish Details, Manage Dish |
| TV2 | Cart & Checkout | Manage Cart, Place Order |
| TV3 | Account | Register, Login, Update Profile, Password Recovery |
| TV4 | Customer / Admin Orders | View Order History, View Order Details, Cancel Order, Manage Orders |
| TV5 | Manage Users & Integration | Manage Users, tích hợp, kiểm thử tổng hợp, đóng gói |

Xem [quy ước nhóm](docs/CONVENTIONS.md) và tài liệu đặc tả trong `docs/` trước khi code. C++ được lựa chọn vì phù hợp Stage 3 của hướng dẫn giáo viên và phần lớn module nhóm đã viết bằng C++.

## Công nghệ / thiết kế

- **C++17 + OOP** (chuẩn đề xuất cần cả nhóm thống nhất); phân tách Presentation → Service → Repository.
- **Build:** có `CMakeLists.txt` dùng để biên dịch/kiểm thử **hai bản demo TV3 riêng biệt** từ ZIP gốc; chưa có executable tích hợp cả nhóm.
- **Testing:** test case Excel; ưu tiên Google Test cho unit test, cộng với integration test và bằng chứng chạy thực tế.
- **GUI:** theo yêu cầu Stage 2 của giáo viên; bản chạy console trong các ZIP hiện là prototype, **chưa thay thế thiết kế GUI**. Cần chốt thư viện GUI hoặc mức độ chấp nhận của giảng viên.
- **Data persistence:** cần cả nhóm chốt MySQL/file trước khi tích hợp. SQL `database/schema.sql` và `seed.sql` là tài liệu/bản mẫu từ nền tảng PHP cũ; **chưa chứng minh kết nối C++ ↔ MySQL**.
- `docker/`, `docker-compose.yml` hiện là bản PHP/MySQL trước khi đổi hướng: **không dùng làm hướng dẫn chạy ứng dụng C++** cho đến khi thay Dockerfile.

## Tổ chức repository

```text
ProgAndTest_Group11/
├── README.md
├── docs/
│   └── CONVENTIONS.md
├── CMakeLists.txt         # build/test 2 demo TV3 độc lập
├── src/                   # C++ tích hợp: CHƯA ĐƯỢC TẠO/HOÀN THÀNH
│   ├── main.cpp           # một entry point duy nhất
│   ├── models/
│   ├── repositories/
│   ├── services/
│   ├── presentation/
│   └── common/
├── tests/
│   ├── unit/              # Google Test hoặc test harness C++ thống nhất
│   ├── integration/
│   ├── testcases/         # test case Excel của từng module
│   └── Testing_Document.xlsx
├── submissions/           # ZIP nguyên gốc thành viên đã nộp để đối chiếu
│   └── tv3/
├── evidence/              # ảnh/log chạy test, PR, Issues (khi có)
├── database/              # schema/seed hiện có, chờ chốt cách lưu trữ
├── docker/                # cấu hình cũ, cần cập nhật cho C++
└── docker-compose.yml     # cấu hình cũ, chưa phải C++ runtime
```

**Lưu ý:** Git không lưu thư mục rỗng; các thư mục `src/` hoặc `evidence/` chỉ xuất hiện sau khi có file thực tế.

## Các bài đã nhận và mức độ tích hợp

- `submissions/tv3/TV3_Account_Submission.zip`: nguồn C++ mẫu về Account, có test và tài liệu; bản console, repository dữ liệu tạm, **chưa tích hợp**.
- `submissions/tv3/TV3_CPP.zip`: nguồn C++ mẫu về Checkout/Orders; cần xác nhận lại phạm vi TV2/TV4, **chưa tích hợp**.
- Các bảng Excel đã nhận được đưa vào `tests/testcases/`, giữ nguyên nội dung. **Không tự động xem tất cả test case là PASS.**
- Những file Excel 1 byte từng được tạo làm placeholder trong `tests/testcases/` **không phải workbook hợp lệ**; cần thay bằng file thực khi hợp nhất. Không xóa nhầm tài liệu thật của thành viên.

## Chạy / build

**Đã kiểm tra CMake cho hai demo TV3 độc lập** (Account và Orders); chưa có chương trình chung của 5 thành viên. Cài CMake >= 3.18 và trình biên dịch C++17, rồi chạy trong `ProgAndTest_Group11`:

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Các lệnh trên đã được kiểm thử với hai ZIP TV3 trong môi trường GNU C++/CMake, cho **2/2 bài test dạng executable PASS**. Chạy với cấu hình Debug vì các test dùng `assert`. Chưa kiểm thử trên Windows hoặc với module khác. Chi tiết: [`docs/CPP_INTEGRATION_STATUS.md`](docs/CPP_INTEGRATION_STATUS.md).

## Quy trình tích hợp / nộp bài

1. Mỗi TV chuẩn hóa nguồn C++ theo `docs/CONVENTIONS.md`, nộp qua nhánh `feature-*` và Pull Request.
2. Đối chiếu interface chung (`User`, `Order`, `Product`), tránh nhiều `main()`, trùng class và tên hàm.
3. Chạy build toàn hệ thống và test; lập Excel test case với Actual Result, Status, Evidence.
4. Ghi Issue lỗi, đính kèm bằng chứng tại `evidence/`; chỉ sau khi review mới merge vào `main`.
5. Sau khi dự án C++ chạy ổn định, cập nhật lại README và Docker, đóng gói theo yêu cầu giảng viên.

Lưu ý: tài liệu đặc tả gốc có thể mô tả giao diện web/PHP; **không tự ý sửa hoặc xóa bản gốc**. Nếu thay đổi công nghệ làm lệch đặc tả, cần ghi nhận trong tài liệu thiết kế bổ sung và thống nhất với nhóm/giảng viên.
