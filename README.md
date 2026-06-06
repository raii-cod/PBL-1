# PBL1: Mô phỏng thuật toán Dijkstra tìm đường đi ngắn nhất

Dự án mô phỏng trực quan từng bước thuật toán Dijkstra tìm đường đi ngắn nhất trên đồ thị có hướng, có trọng số. Đồ án được thực hiện trong học phần Đồ án Lập trình Tính toán (PBL1) tại Trường Đại học Bách Khoa - ĐHĐN.

Ngôn ngữ lập trình: **C++**
Thư viện đồ họa: **Raylib**

---

## 📌 Tính năng chính

- **Mô phỏng thuật toán trực quan:** Hiển thị từng bước duyệt đồ thị và cập nhật khoảng cách.
- **Tối ưu hóa bằng Min-Heap:** Thuật toán Dijkstra được tối ưu tốc độ bằng cấu trúc Hàng đợi ưu tiên (Min-Heap).
- **Sắp xếp đỉnh tự động:** Đọc dữ liệu từ file văn bản và tự động vẽ các đỉnh dàn đều theo hình tròn.
- **Nhật ký hoạt động (Log):** Hiển thị chi tiết lịch sử xử lý và các trạng thái bên phải màn hình.
- **Màu sắc dễ nhìn:**
  - `Màu Cam`: Đỉnh bắt đầu.
  - `Màu Vàng`: Đỉnh đang được xét.
  - `Màu Xanh lá`: Đỉnh đã tìm được khoảng cách ngắn nhất.
  - `Màu Đỏ`: Đường đi ngắn nhất cuối cùng khi kết thúc.

---

## 🎮 Phím tắt điều khiển

- `SPACE`: Bắt đầu chạy thuật toán.
- `R`: Khôi phục đồ thị về trạng thái ban đầu.
- `N`: Đóng cửa sổ đồ họa hiện tại để nhập file dữ liệu mới.
- `ESC`: Thoát chương trình.

---

## ⚙️ Cài đặt và biên dịch

**Yêu cầu hệ thống:**
1. Trình biên dịch C/C++ (GCC/MinGW).
2. Thư viện đồ họa **Raylib**.

**Cách biên dịch và chạy chương trình:**

Mở Terminal/Command Prompt tại thư mục dự án và chạy lệnh sau để biên dịch:

```bash
mingw32-make 
# Hoặc:
# make
```

Sau khi biên dịch xong, khởi chạy chương trình bằng lệnh:

```bash
./game
```
