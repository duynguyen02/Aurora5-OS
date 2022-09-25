# Bài tập lớn môn Nguyên lý hệ điều hành: Aurora5-OS
![aurora5_os](/img/aurora5_os.png)
## Mục lục
- [Đề tài](#đề-tài)
- [Mô tả](#mô-tả)
- [Cài đặt và sử dụng](#cài-đặt-và-sử-dụng)
    - [Yêu cầu hệ điều hành](#yêu-cầu-hệ-điều-hành)
    - [Công cụ và thư viện](#công-cụ-và-thư-viện-thực-hiện-các-câu-lệnh-bên-dưới-nếu-trong-quá-trình-chạy-bị-lỗi)
    - [Cài đặt Aurora5-OS](#cài-đặt-aurora5-os)
    - [Cài đặt các câu lệnh cho Shell](#cài-đặt-các-câu-lệnh-cho-Shell)

## Đề tài
- Aurora5-OS: mô phỏng Shell cho 1 hệ điều hành đơn giản.
- Ngôn ngữ: C

## Tác giả
`Nguyễn Văn Khánh Duy - 2051067158`
>[![Facebook](https://img.shields.io/badge/Facebook-1877F2?style=for-the-badge&logo=facebook&logoColor=white)](https://www.facebook.com/khanhduy.tls.4/)

`Nguyễn Hoài Nguyệt An`
>[![Facebook](https://img.shields.io/badge/Facebook-1877F2?style=for-the-badge&logo=facebook&logoColor=white)](https://www.facebook.com/profile.php?id=100012224136535)

`Nguyền Phạm Xuân Hiền`
>[![Facebook](https://img.shields.io/badge/Facebook-1877F2?style=for-the-badge&logo=facebook&logoColor=white)](https://www.facebook.com/profile.php?id=100013313088490)

`Nguyền Phùng Hưng - 2051067550`
>[![Facebook](https://img.shields.io/badge/Facebook-1877F2?style=for-the-badge&logo=facebook&logoColor=white)](https://www.facebook.com/profile.php?id=100013313088490)

`Trần Văn Tân - 2051067560`
>[![Facebook](https://img.shields.io/badge/Facebook-1877F2?style=for-the-badge&logo=facebook&logoColor=white)](https://www.facebook.com/Augenstern.jun03)

## Mô tả
- Mô phỏng 1 Shell đơn giản để thực hiện tương tác giữa người với hệ điều hành.
- Shell thực hiện tương tác bán độc lập: các thao tác câu lệnh chỉ thực hiện trong phạm vi thư mục đang chứa Shell. Shell và các câu lệnh được viết dựa trên các thư viện C thuộc Linux (Linux API).
- Các câu lệnh phụ thuộc vào Shell: đối số dòng lệnh từ Shell truyền vào tệp thực thi luôn chứa thư mục của đường dẫn tới Shell ở đối số cuối cùng.

## Cấu trúc thư mục và tệp tin Aurora5-OS
![aurora5_os_structure](/img/aurora5_os_struct.png)
### Thư mục /bin
- Chứa các tệp thực thi cho Shell.
### Thư mục /etc
- Chứa các tệp cấu hình cho Aurora5-OS và tệp cấu hình cho các tệp thực thi (nếu có).
### Thư mục /home
- Chứa các thư mục dành cho các người dùng (trừ root).
### Thư mục /root
- Thư mục dành cho root.
### Tệp AURORA5_OS
- Tệp thực thi Aurora5-OS Shell.
### Tệp etc/hostname
- Chứa tên máy chủ.
### Tệp etc/passwd
- Chứa thông tin các người dùng.
### Tệp etc/usershell.bin
- Chứa thông tin các người dùng đang đăng nhập vào Shell.
## Cơ chế của Aurora5-OS
### Shell
![aurora5_os](/img/aurora5_os_shell_mach.png)
- Exit code: mỗi câu lệnh khi vào trạng thái kết thúc sẽ trả về 1 con số biểu hiện trạng thái thực thi của câu lệnh bao gồm: 128(kết thúc chương trình), 0(hoàn thành), 1(có lỗi đã xảy ra), 2(thiếu các đối số cần thiết), 126(câu lệnh không thể thực thi), 127(không tìm thấy câu lệnh) và sẽ được biểu hiện qua ký hiệu `✓` (thành công) và `✗` (có lỗi xảy ra)
- Tên máy chủ: hiển thị tên máy chủ được lưu trữ trong tệp [/etc/hostname](#cấu-trúc-thư-mục-và-tệp-tin-aurora5-os) 
- Tên người dùng hiện hành: hiển thị tên của người dùng đang đăng nhập vào Shell được lưu trữ trong tệp  [/etc/usershell.bin](#cấu-trúc-thư-mục-và-tệp-tin-aurora5-os) 
- Tên thư mục hiện hành: thư mục người dùng đang đăng nhập vào Shell trỏ tới được lưu trữ trong tệp [/etc/usershell.bin](#cấu-trúc-thư-mục-và-tệp-tin-aurora5-os)
- Vùng gõ câu lệnh: các câu lệnh để tương tác giữa người dùng và hệ điều hành được nhập tại đây, đối số dòng lệnh cuối cùng luôn được Shell tự động truyền vào là đường dẫn tuyệt đối từ máy chính đến Aurora5-OS Shell.
### Lưu trữ người dùng
- Tất cả người dùng được tạo ra trong Aurora5-OS được lưu trữ trong tệp [/etc/passwd](#cấu-trúc-thư-mục-và-tệp-tin-aurora5-os) với người dùng root luôn đứng đầu.
- Mỗi người dùng được lưu trữ trong tệp [/etc/passwd](#cấu-trúc-thư-mục-và-tệp-tin-aurora5-os) theo từng dòng. Mỗi thông tin của người dùng được ngăn cách với nhau thông qua dấu `:` bao gồm tên người dùng, mật khẩu (được mã hóa theo chuẩn SHA-512), quyền hạn của người dùng (1: người dùng quản trị, 0: người dùng bình thường)
![aurora5_os_structure](/img/passwd_file_ex.png)
### Lưu trữ người dùng đăng nhập Shell
- Khi một người dùng đăng nhập vào Shell, thông tin của người dùng sẽ được lưu trữ dưới dạng cấu trúc nhị phân trong tệp [/etc/usershell.bin](#cấu-trúc-thư-mục-và-tệp-tin-aurora5-os)
- Một Shell chỉ có thể có một người dùng sử dụng nhưng có thể đăng nhập nhiều người dùng cùng lúc và được lưu trữ dưới dạng cấu trúc nhị phân trong tệp [/etc/usershell.bin](#cấu-trúc-thư-mục-và-tệp-tin-aurora5-os) với cơ chế `Stack` (vào trước ra sau).

![usershell_bin_stack](/img/usershell_bin_stack.png)

![shell_user_stack_ex](/img/shell_user_stack_ex.png)

### Tệp /etc/usershell.bin
- Lưu trữ thông tin người dùng dưới dạng cấu trúc nhị phân.
- Thông tin lưu trữ bao gồm: tên người dùng, đường dẫn người dùng đang trỏ tới, vai trò người dùng và đuờng dẫn tuyệt đối.

### Sự phụ thuộc của các tệp thực thi
- Các tệp thực thi được sử dụng trong Aurora5-OS Shell phải tuân thủ các quy tắc.
- Đối số dòng lệnh truyền vào từ Aurora5-OS Shell vào tệp thực thi luôn đi kèm đường dẫn tuyệt đối từ máy chính tới thư mục chứa Aurora5-OS ở đối số dòng lệnh cuối cùng.
- Các tệp thực thi phải được xây dựng dựa trên ngôn ngữ C/C++ và chỉ sử dụng Linux API và "Aurora5-OS API" (sử dụng các hàm để lấy thông tin người dùng từ shell, thêm người dùng vào shell,...)

### Cơ chế thực thi dòng lệnh của Aurora5-OS Shell
- Câu lệnh truyền và Shell phải tuân theo `MAX_BUFFER_SIZE` là `255`, nếu
câu lệnh vượt quá số byte trên Aurora5-OS Shell có thể bị Crash.
- Nếu người dùng không nhập gì cả thì Shell sẽ trả về  `SUCESS_EXIT_STATUS` (tức là câu lệnh hoàn thành với Exit code là 0).
- Nếu người dùng có nhập câu lệnh, câu lệnh sẽ được đưa vào bộ xử lý câu lệnh để phân chia thành các đối số và thêm đường dẫn tuyệt đối từ máy chính đến Aurora5-OS ở đối số cuối cùng.
- Aurora5-OS sẽ bắt đầu tiềm kiếm câu lệnh trong đối số dòng lệnh đầu tiên trong thư mục [/bin](#cấu-trúc-thư-mục-và-tệp-tin-aurora5-os)
- Nếu câu lệnh không được tìm thấy, Shell sẽ trả về `CMD_NOT_FOUND_EXIT_STATUS` (tức là câu lệnh không được tìm thấy với mã lỗi là 127)
- Nếu câu lệnh được tìm thấy, toàn bộ câu lệnh và đối số được chuyển vào bộ tạo tiến trình để câu lệnh được thực thi trong một tiến trình mới.
- Sau khi câu lệnh được thực thi xong, tiến trình con được ngắt, câu lệnh sẽ trả về một [Exit code](#shell) tương ứng.

![shell_exe_ex](/img/shell_exe_ex.png)


## Cài đặt và sử dụng
### Yêu cầu hệ điều hành.
- Ubuntu và các distro tương tự (Debian, Lubuntu, Bodhi,... )
### Công cụ và thư viện (thực hiện các câu lệnh bên dưới nếu trong quá trình chạy bị lỗi)
- `gcc (Ubuntu 11.2.0-19ubuntu1) 11.2.0`
- `GNU ld (GNU Binutils for Ubuntu) 2.38`
- `cmake phiên bản 3.0.0 trở lên` 
```
sudo apt install cmake
```
- `build-essential`
```
sudo apt install build-essential
```
- `GNU Make 4.3`
```
sudo apt install make
```
- `Thư viện curses.h`
```
sudo apt-get install libncurses-dev
```
- `Thư viện curl.h`
```bash
sudo apt install libcurl4-gnutls-dev
```
### Cài đặt Aurora5-OS
- `Tải mã nguồn và vào thư mục chứa mã nguồn`
```bash
git clone https://github.com/duynguyen02/Aurora5-OS
cd Aurora5-OS/src
```
- `Khởi tạo Makefile để biên dịch mã nguồn`
```bash
sh ./install.sh
```
- `Vào thư mục chứa Makefile và tiến hành biên dịch mã nguồn`
```bash
cd out/build
make
```
- `Khởi chạy Aurora5-OS`
```
./AURORA5_OS
```
- `Gỡ cài đặt Aurora5-OS`
```bash
# trở về thư mục mã nguồn Aurora5-OS
cd ../..
# gỡ cài đặt Aurora5-OS
sh ./unistall.sh
```
### Cài đặt các câu lệnh cho Shell
Để có thể thực hiện các câu lệnh trong Aurora5-OS Shell, chúng ta mở một Terminal mới, vào lại thư mục Aurora5-OS và thực hiện các câu lệnh sau:
- `Vào thư mục chứa mã nguồn các câu lệnh và biên dịch thành các tệp thực thi`
```bash
cd src/commands
sh ./build_all_script.sh
```
- `Cài đặt các tệp thực thi vào Aurora5-OS Shell`
```bash
sh ./install_scripts_to_os.sh
```
- `Xóa các tệp không thiết`
```bash
sh ./remove_all_builded_files.sh
```


