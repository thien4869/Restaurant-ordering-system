-- Dữ liệu mẫu tối thiểu. TV1 bổ sung món ăn, TV3 thêm tài khoản (mật khẩu phải là hash).
SET NAMES utf8mb4;

INSERT INTO provinces (name) VALUES ('Ho Chi Minh City'), ('Ha Noi');
INSERT INTO districts (province_id, name) VALUES (1, 'District 1'), (1, 'District 3'), (2, 'Hoan Kiem');
INSERT INTO wards (district_id, name) VALUES (1, 'Ben Nghe'), (1, 'Ben Thanh'), (2, 'Ward 6'), (3, 'Hang Bac');

INSERT INTO shippings (type, cost) VALUES ('Standard', 15000), ('Express', 30000);

INSERT INTO categories (name) VALUES ('Main Course'), ('Appetizer'), ('Dessert'), ('Drink');
