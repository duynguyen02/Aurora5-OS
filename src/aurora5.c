#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <string.h>

#include "libs/constants.h"

#include "libs/os/shell.h"
#include "libs/os/first_setup.h"
#include "libs/utils/common.h"
#include "libs/user/users_manager.h"

const char *OS_NAME = "Aurora5-OS";

const char *OS_VERSION = "1.0.0";

int main()
{
    // hiển thị tin nhắn chào mừng
    welcome_message(OS_VERSION);

    // lấy thư mục hiện hành (giả định là thư mục gốc)
    char *ROOT_DIR = get_current_dir();

    // thiết lập thư mục hiện hành cho hệ điều hành là thư mục gốc
    char *CURRENT_DIR = ROOT_DIR;

    // nếu thư mục etc chưa tồn tại, giả định hệ điều hành chưa được cài đặt
    // bắt đầu thiết lập cài đặt hệ điều hành
    char *etc_absolute_path;
    etc_absolute_path = malloc(strlen(ROOT_DIR) + strlen(ETC_DIR) + 1);
    strcpy(etc_absolute_path, ROOT_DIR);
    strcat(etc_absolute_path, ETC_DIR);

    if (is_dir_exists(etc_absolute_path) == 0)
    {
        first_time_setup(
            OS_NAME,
            OS_VERSION,
            ROOT_DIR);
        clear_c();
    }

    free(etc_absolute_path);

    // Đăng nhập vào hệ thống

    char *user_name;
    int _is_user_exist = 0;

    while (_is_user_exist == 0)
    {
        user_name = (char *)calloc(MAX_BUFFER_SIZE, sizeof(char));
        printf("Login: ");
        scanf("%s", user_name);
        _is_user_exist = is_user_exist(user_name, ROOT_DIR);

        if (_is_user_exist == 0)
        {
            printf("User does not exist! Please try again!\n");
        }
    }

    char *password;
    int _is_correct_pass = 0;
    while (_is_correct_pass == 0)
    {
        password = (char *)calloc(MAX_BUFFER_SIZE, sizeof(char));
        printf("Password: ");
        password = strdup(getpass(""));
        _is_correct_pass = is_correct_password(user_name, password, ROOT_DIR);
        if (_is_correct_pass == 0)
        {
            printf("Incorrect password! Try again!\n");
        }
    }

    // khởi tạo người dùng hiện tại
    char *CURRENT_USER = user_name;

    free(user_name);

    // cấp phát bộ nhớ cho câu lệnh
    char *command = (char*)calloc(MAX_BUFFER_SIZE,sizeof(char));

    

    return 0;
}