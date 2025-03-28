# KTLT thuc hanh (C++)

## Quản Lý Thông Tin Sinh Viên

### Cấu Trúc SINHVIEN

-   **MaSV**
-   **GioiTinh**
-   **NamSinh**
-   **HoTenSV**
-   **TrungbinhHK**

### Cấu Trúc DIEMSO

-   **MaSV**
-   **KTLT**
-   **MMT**
-   **CTDL**

### Cấu Trúc DANGNHAP

-   **MaSV**
-   **MATKHAU**

#### Khi khởi tạo:

-   Chương trình sẽ đọc file `thongtinSV.txt`, `diemso.txt`, `dangnhap.txt` và lưu vào trong chương trình.

#### Đối chiếu vào MaSV:

Hãy viết hàm tính điểm trung bình học kỳ cho từng sinh viên với công thức sau:  
`(KTLT*4 + MMT*3 + CTDL*3) / 10`  
Giá trị tính được lưu vào **TrungbinhHK** tương ứng với sinh viên đó.

#### Hãy viết Hàm:

-   Cho người dùng đăng nhập tài khoản vào hệ thống.
-   Sử dụng dữ liệu đã lấy từ file `dangnhap.txt` để xác thực.
-   Nếu người dùng nhập sai, thông báo **Sai** và yêu cầu người dùng nhập lại.
-   Nếu nhập sai quá 5 lần thì thoát chương trình.

---

## Khi người dùng đăng nhập thành công:

### Nếu tài khoản đăng nhập là **sinh viên** (MaSV khác 11377):

-   Xuất ra màn hình các thông tin cá nhân và điểm số của sinh viên.

### Nếu tài khoản đăng nhập là **giảng viên** (MaSV là 11377):

1. Thêm sinh viên.
2. Cập nhật thông tin sinh viên.
3. Xóa sinh viên.
4. Tìm kiếm và hiển thị thông tin sinh viên theo **MaSV**.
5. Hiển thị danh sách sinh viên **không bị cảnh báo học tập** (TrungbinhHK >= 4).
6. Xuất danh sách sinh viên ra file `thongtinSV_export.txt`.
7. ... _Sinh viên sáng tạo thêm chức năng._

---

### **_Lưu ý:_**

-   _Với các yêu cầu **chữ màu đỏ**, các bạn có thể chọn làm những chức năng khác thay thế theo sở thích và tính sáng tạo của các bạn._
-   _Với các yêu cầu **nền màu vàng**, các bạn có thể bỏ qua nếu không làm được._

### Lưu **_Lưu ý:_**

-   **chữ màu đỏ**

```
3. Xóa sinh viên.
4. Tìm kiếm và hiển thị thông tin sinh viên theo **MaSV**.
5. Hiển thị danh sách sinh viên **không bị cảnh báo học tập** (TrungbinhHK >= 4).
```

```
7. ... *Sinh viên sáng tạo thêm chức năng.*
```

-   **nền màu vàng**

```
Khi khởi tạo:

-   Chương trình sẽ đọc file `thongtinSV.txt`, `diemso.txt`, `dangnhap.txt` và lưu vào trong chương trình.
```

```
Hãy viết Hàm:

-   Cho người dùng đăng nhập tài khoản vào hệ thống.
-   Sử dụng dữ liệu đã lấy từ file `dangnhap.txt` để xác thực.
-   Nếu người dùng nhập sai, thông báo **Sai** và yêu cầu người dùng nhập lại.
-   Nếu nhập sai quá 5 lần thì thoát chương trình.
```
