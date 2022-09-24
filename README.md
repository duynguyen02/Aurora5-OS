# Bài tập lớn môn Nguyên lý hệ điều hành: Aurora5-OS

## Mục lục
- [Đề tài](#đề-tài)
- [Mô tả](#mô-tả)
- [Cài đặt và sử dụng](#cài-đặt-và-sử-dụng)
    - [Yêu cầu hệ điều hành](#yêu-cầu-hệ-điều-hành)
    - [Công cụ và thư viện](#công-cụ-và-thư-viện-thực-hiện-các-câu-lệnh-bên-dưới-nếu-trong-quá-trình-chạy-bị-lỗi)
    - [Cài đặt Aurora5-OS](#cài-đặt-aurora5-os)
    - [Cài đặt các câu lệnh cho Shell](#cài-đặt-các-câu-lệnh-cho-shell)

## Đề tài:
- Aurora5-OS: mô phỏng shell cho 1 hệ điều hành đơn giản.
- Ngôn ngữ: C

## Mô tả:
- Mô phỏng 1 Shell đơn giản để thực hiện tương tác giữa người với hệ điều hành.
- Shell thực hiện tương tác bán độc lập: các thao tác câu lệnh chỉ thực hiện trong phạm vi thư mục đang chứa shell. Shell và các câu lệnh được viết dựa trên các thư viện C thuộc Linux (Linux API).
- Các câu lệnh phụ thuộc vào shell: đối số dòng lệnh từ shell truyền vào file thực thi luôn chứa thư mục của đường dẫn tới shell ở đối số cuối cùng.

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


