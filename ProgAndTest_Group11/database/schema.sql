-- Theo ERD của nhóm. Tên bảng số nhiều, snake_case (xem docs/CONVENTIONS.md).
-- Bổ sung: products.discount, orders.payment_status.
SET NAMES utf8mb4;

CREATE TABLE provinces (
  province_id INT AUTO_INCREMENT PRIMARY KEY,
  name        VARCHAR(100) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE districts (
  district_id INT AUTO_INCREMENT PRIMARY KEY,
  province_id INT NOT NULL,
  name        VARCHAR(100) NOT NULL,
  FOREIGN KEY (province_id) REFERENCES provinces(province_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE wards (
  ward_id     INT AUTO_INCREMENT PRIMARY KEY,
  district_id INT NOT NULL,
  name        VARCHAR(100) NOT NULL,
  FOREIGN KEY (district_id) REFERENCES districts(district_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE shippings (
  shipping_id INT AUTO_INCREMENT PRIMARY KEY,
  type        VARCHAR(50) NOT NULL,
  cost        DECIMAL(12,0) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE users (
  email    VARCHAR(100) PRIMARY KEY,
  password VARCHAR(255) NOT NULL,          -- lưu hash, không lưu chữ thường
  name     VARCHAR(100) NOT NULL,
  address  VARCHAR(255),
  mobile   VARCHAR(20),
  dob      DATE,
  gender   VARCHAR(10)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE administrators (
  username  VARCHAR(50) PRIMARY KEY,
  password  VARCHAR(255) NOT NULL,         -- lưu hash
  name      VARCHAR(100) NOT NULL,
  privilege VARCHAR(50)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE categories (
  category_id INT AUTO_INCREMENT PRIMARY KEY,
  name        VARCHAR(100) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE products (
  code        VARCHAR(20) PRIMARY KEY,
  category_id INT NOT NULL,
  name        VARCHAR(150) NOT NULL,
  image       VARCHAR(255),
  price       DECIMAL(12,0) NOT NULL CHECK (price >= 0),
  discount    INT NOT NULL DEFAULT 0 CHECK (discount BETWEEN 0 AND 100),
  status      VARCHAR(20) NOT NULL DEFAULT 'AVAILABLE',   -- AVAILABLE, SOLD_OUT, DISCONTINUED
  description TEXT,
  detail      TEXT,
  views       INT NOT NULL DEFAULT 0,
  sold        INT NOT NULL DEFAULT 0,
  FOREIGN KEY (category_id) REFERENCES categories(category_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE carts (
  cart_id    INT AUTO_INCREMENT PRIMARY KEY,
  user_email VARCHAR(100) NOT NULL,
  created_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (user_email) REFERENCES users(email)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE cart_items (
  cart_id      INT NOT NULL,
  product_code VARCHAR(20) NOT NULL,
  qty          INT NOT NULL CHECK (qty > 0),
  PRIMARY KEY (cart_id, product_code),
  FOREIGN KEY (cart_id) REFERENCES carts(cart_id) ON DELETE CASCADE,
  FOREIGN KEY (product_code) REFERENCES products(code)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE orders (
  order_number     VARCHAR(20) PRIMARY KEY,                -- ORD-YYYYMMDD-NNNN
  user_email       VARCHAR(100) NOT NULL,
  shipping_id      INT NOT NULL,
  ward_id          INT NOT NULL,
  receiver_name    VARCHAR(100) NOT NULL,
  receiver_mobile  VARCHAR(20) NOT NULL,
  receiver_address VARCHAR(255) NOT NULL,
  order_datetime   DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  total            DECIMAL(12,0) NOT NULL,
  payment_type     VARCHAR(10) NOT NULL,                   -- CASH, ONLINE
  payment_status   VARCHAR(10) NOT NULL DEFAULT 'PENDING', -- PENDING, PAID, FAILED
  status           VARCHAR(15) NOT NULL DEFAULT 'NEW',     -- NEW, CONFIRMED, IN_TRANSIT, COMPLETED, CANCELED
  FOREIGN KEY (user_email)  REFERENCES users(email),
  FOREIGN KEY (shipping_id) REFERENCES shippings(shipping_id),
  FOREIGN KEY (ward_id)     REFERENCES wards(ward_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE order_details (
  order_number VARCHAR(20) NOT NULL,
  product_code VARCHAR(20) NOT NULL,
  qty          INT NOT NULL CHECK (qty > 0),
  price        DECIMAL(12,0) NOT NULL,                     -- giá tại thời điểm đặt
  PRIMARY KEY (order_number, product_code),
  FOREIGN KEY (order_number) REFERENCES orders(order_number),
  FOREIGN KEY (product_code) REFERENCES products(code)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
