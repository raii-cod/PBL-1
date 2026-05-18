# PBL1: Xây dựng Chương trình Tìm Đường đi Ngắn nhất trên Đồ thị Có hướng Có trọng số

Một ứng dụng mô phỏng trực quan tương tác từng bước (*Step-by-Step*) thuật toán Dijkstra giải bài toán đường đi ngắn nhất trên cấu trúc đồ thị định hướng. Dự án được phát triển trong khuôn khổ học phần Đồ án Lập trình Tính toán (PBL1) tại Trường Đại học Bách Khoa - ĐHĐN.

Chương trình kết hợp sức mạnh tính toán tối ưu mảng tĩnh của ngôn ngữ **C++** với thư viện kết xuất đồ họa thời gian thực **Raylib** để trực quan hóa toàn bộ tiến trình loang nhãn tối ưu toán học.

---

## 🌟 Các Tính Năng Cốt Lõi

* **Giao diện dòng lệnh khởi tạo (Console)**: Xuất màn hình giới thiệu định dạng căn lề trung tâm chuẩn hóa thông tin học phần.
* **Đọc dữ liệu từ file cấu trúc tĩnh**: Tự động phân tích tệp cấu trúc bao gồm số đỉnh, ma trận kề chi phí và cặp đỉnh nguồn/đích.
* **Quy hoạch phân bổ đa giác đều**: Tự động tính toán phương trình tham số đường tròn để dàn đều vị trí tọa độ các đỉnh, giảm thiểu hiện tượng chồng chéo đường nối.
* **Máy trạng thái mô phỏng (State Machine)**: Phân rã logic thuật toán Dijkstra thành các trạng thái độc lập (`STATE_FIND_MIN`, `STATE_RELAX_EDGES`, `STATE_FINISHED`) để không làm đóng băng luồng dựng hình đồ họa.
* **Bảng nhật ký hoạt động thời gian thực**: Khung điều khiển bên phải lưu trữ bộ đệm hiển thị lên đến 20 dòng lịch sử chi tiết tiến trình tối ưu nhãn ứng với từng chu kỳ thời gian trễ.
* **Đồ họa hướng vector độ chính xác cao**: Ứng dụng toán lượng giác (`atan2f`) để tính điểm dừng biên của mũi tên chạm vừa khít rìa ngoài nút tròn.
* **Phản hồi trạng thái qua màu sắc**:
  * Đỉnh nguồn: Màu Cam (`ORANGE`).
  * Đỉnh đang đóng vai trò tâm loang xét duyệt hiện tại: Màu Vàng (`YELLOW`).
  * Đỉnh đã tối ưu hoàn toàn (cố định nhãn): Màu Xanh lá (`LIME`).
  * Lộ trình ngắn nhất cuối cùng: Tô sắc Đỏ (`RED`) rực rỡ với độ dày nét vượt trội.

---

## 🎮 Cơ Chế Điều Khiển Tương Tác (Keybindings)

Khi cửa sổ đồ họa Raylib được kích hoạt, ông có thể điều phối tiến trình thông qua hệ thống phím tắt:

* `SPACE`: Bắt đầu kích hoạt luồng chạy mô phỏng máy trạng thái thuật toán.
* `R`: Tiến hành đặt lại toàn bộ thông số cấu trúc nhãn về trạng thái ban đầu để theo dõi lại từ đầu.
* `N`: Đóng cửa sổ đồ họa hiện tại để quay về màn hình Console nạp một tệp tin dữ liệu đồ thị mới.
* `ESC`: Thoát hoàn toàn khỏi chương trình hệ thống.

---

## ⚙️ Yêu Cầu Hệ Thống & Cài Đặt Phụ Thuộc

Để biên dịch và chạy được dự án này, máy tính của ông cần cài đặt sẵn:

1. **Trình biên dịch C/C++**: GCC/MinGW (Hỗ trợ tiêu chuẩn C++11 trở lên).
2. **Thư viện Raylib**: Thư viện đồ họa tương tác đa nền tảng.

### Hướng dẫn biên dịch nhanh bằng G++ (giao diện dòng lệnh Compiler):

```bash
g++ main.cpp -o main -lraylib -lopengl32 -lgdi32 -lwinmm