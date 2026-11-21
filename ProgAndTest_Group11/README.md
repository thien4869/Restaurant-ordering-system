# Restaurant Order Management System – Group 11

Hệ thống đặt món cho nhà hàng: khách xem/tìm thực đơn, thêm món vào giỏ, đặt món, theo dõi đơn; admin quản lý món, người dùng và đơn hàng.

Công nghệ theo đặc tả: HTML, CSS, JS, PHP, MySQL. Đóng gói bằng Docker.

> Các sơ đồ dưới đây viết bằng Mermaid, GitHub tự hiển thị. Sơ đồ gốc (use-case, lớp, ERD, DFD, kiến trúc) nằm trong `docs/Restaurant_System_Specification_v1_2_edited.docx`.

## Thành viên và phân công

| TV | Họ tên | Module | Use-case |
|---|---|---|---|
| TV1 | _điền tên_ | Thực đơn | Browse Menu, Search Dish, View Dish Details, Manage Dish |
| TV2 | _điền tên_ | Giỏ & Đặt món | Manage Cart, Checkout / Place Order |
| TV3 | _điền tên_ | Tài khoản | Register, Login, Update Profile, Password Recovery |
| TV4 | _điền tên_ | Đơn hàng | View Order History, View Order Details, Cancel Order, Manage Orders (admin) |
| TV5 | _điền tên_ | Người dùng & Hạ tầng | Manage Users, repo, CSDL, Docker, đóng gói nộp bài |

Chi tiết từng đầu việc: `docs/Phan_Cong_Nhiem_Vu_Group11.xlsx`. Quy ước chung: `docs/CONVENTIONS.md`.

## Cách chạy

```bash
docker compose up --build
```

Mở http://localhost:8080. CSDL tự tạo từ `database/schema.sql` và `database/seed.sql` ở lần chạy đầu. Muốn tạo lại CSDL từ đầu: `docker compose down -v` rồi chạy lại lệnh trên.

File Docker là bản mẫu cho PHP + MySQL. Nếu nhóm đổi ngôn ngữ thì TV5 sửa `docker/Dockerfile`.

## Sơ đồ

### 1. Use-case theo người phụ trách

```mermaid
flowchart LR
    Guest([Guest])
    Customer([Customer])
    Admin([Admin])

    subgraph TV1["TV1 – Thực đơn"]
        UC1[Browse Menu]
        UC2[Search Dish]
        UC3[View Dish Details]
        UC4[Manage Dish]
    end
    subgraph TV2["TV2 – Giỏ & Đặt món"]
        UC5[Manage Cart]
        UC6[Checkout / Place Order]
    end
    subgraph TV3["TV3 – Tài khoản"]
        UC7[Register]
        UC8[Login]
        UC9[Update Profile]
        UC10[Password Recovery]
    end
    subgraph TV4["TV4 – Đơn hàng"]
        UC11[View Order History]
        UC12[View Order Details]
        UC13[Cancel Order]
        UC15[Manage Orders]
    end
    subgraph TV5["TV5 – Người dùng"]
        UC14[Manage Users]
    end

    Guest --> UC1 & UC2 & UC3 & UC5 & UC7 & UC8
    UC8 -.->|extend| UC10
    Customer -->|"kế thừa Guest"| Guest
    Customer --> UC6 & UC9 & UC11 & UC12 & UC13
    Admin --> UC4 & UC8 & UC14 & UC15
```

### 2. Kiến trúc 3 lớp

```mermaid
flowchart TB
    Users(["Guest · Customer · Admin"])

    subgraph L1["Lớp 1 – Presentation"]
        CW["Customer Web<br/>Menu · Cart · Order History · Details · Cancel · Place Order"]
        AD["Admin Dashboard<br/>Order List · Search · Filter · Details · Update Status"]
    end
    subgraph L2["Lớp 2 – Business Logic"]
        AS["Account Service<br/>Login · Profile · Permissions"]
        MS["Menu & Cart Service<br/>Browse Menu · Manage Cart"]
        OS["Order Service<br/>Place · View · Cancel · Manage"]
    end
    subgraph L3["Lớp 3 – Data Access"]
        UR["User Repository"]
        MR["Menu & Cart Repository"]
        PR["Payment Repository"]
        ORP["Order Repository"]
    end
    DB[("Database<br/>Users · Menu · Cart · Orders · OrderDetails · Payment")]

    Users --> CW & AD
    CW --> AS & MS & OS
    AD --> OS
    AS --> UR
    MS --> MR
    OS --> ORP & PR
    UR & MR & PR & ORP --> DB
```

### 3. Cơ sở dữ liệu (ERD)

Theo ERD của nhóm, đổi tên bảng sang số nhiều (xem `docs/CONVENTIONS.md`), có thêm `products.discount` và `orders.payment_status`.

```mermaid
erDiagram
    provinces ||--o{ districts : contains
    districts ||--o{ wards : contains
    categories ||--o{ products : "belongs to"
    users ||--o{ orders : places
    users ||--o| carts : owns
    carts ||--o{ cart_items : contains
    products ||--o{ cart_items : "in"
    orders ||--|{ order_details : "consists of"
    products ||--o{ order_details : "ordered in"
    shippings ||--o{ orders : uses
    wards ||--o{ orders : "delivered to"

    provinces {
        int province_id PK
        string name
    }
    districts {
        int district_id PK
        int province_id FK
        string name
    }
    wards {
        int ward_id PK
        int district_id FK
        string name
    }
    shippings {
        int shipping_id PK
        string type
        decimal cost
    }
    categories {
        int category_id PK
        string name
    }
    products {
        string code PK
        int category_id FK
        string name
        string image
        decimal price
        int discount "them moi"
        string status
        string description
        string detail
        int views
        int sold
    }
    users {
        string email PK
        string password "luu hash"
        string name
        string address
        string mobile
        date dob
        string gender
    }
    administrators {
        string username PK
        string password "luu hash"
        string name
        string privilege
    }
    carts {
        int cart_id PK
        string user_email FK
        datetime created_at
    }
    cart_items {
        int cart_id PK
        string product_code PK
        int qty
    }
    orders {
        string order_number PK
        string user_email FK
        int shipping_id FK
        int ward_id FK
        string receiver_name
        string receiver_mobile
        string receiver_address
        datetime order_datetime
        decimal total
        string payment_type
        string payment_status "them moi"
        string status
    }
    order_details {
        string order_number PK
        string product_code PK
        int qty
        decimal price
    }
```

### 4. Trạng thái đơn hàng

```mermaid
stateDiagram-v2
    state "IN_TRANSIT" as InTransit
    [*] --> NEW : Checkout thành công
    NEW --> CONFIRMED : Admin xác nhận
    CONFIRMED --> InTransit : Admin cập nhật
    InTransit --> COMPLETED : Admin cập nhật
    NEW --> CANCELED : Khách hủy (chỉ khi còn NEW)
    COMPLETED --> [*]
    CANCELED --> [*]
```

### 5. Quy trình Git

```mermaid
gitGraph
    commit id: "init repo"
    branch feature-menu
    commit id: "TV1 menu"
    checkout main
    branch feature-cart-checkout
    commit id: "TV2 cart + checkout"
    checkout main
    branch feature-account
    commit id: "TV3 account"
    checkout main
    branch feature-orders
    commit id: "TV4 orders"
    checkout main
    branch feature-users
    commit id: "TV5 users"
    checkout main
    merge feature-account
    merge feature-menu
    merge feature-cart-checkout
    merge feature-orders
    merge feature-users
    commit id: "test tích hợp" tag: "nộp bài"
```

## Cấu trúc thư mục

```
ProgAndTest_Group11/
├── src/
│   ├── presentation/        # trang, controller (customer/, admin/)
│   ├── services/            # AccountService, MenuCartService, OrderService
│   ├── repositories/        # UserRepository, MenuCartRepository, OrderRepository
│   ├── models/              # User, Product, Category, Cart, Order, OrderDetail, ...
│   └── common/              # kết nối CSDL, tính giá sau giảm, session
├── tests/
│   ├── unit/                # unit test
│   └── Testing_Document.xlsx
├── database/                # schema.sql, seed.sql
├── docker/                  # Dockerfile
├── docker-compose.yml
├── docs/                    # đặc tả, quy ước, bảng phân công
├── evidence/                # bằng chứng dùng công cụ và phân công
│   ├── commits/
│   ├── tool-usage/
│   └── task-assignment/
└── .github/                 # mẫu Pull Request và báo lỗi
```
