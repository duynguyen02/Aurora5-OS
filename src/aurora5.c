/**  ___                            _____        _____ _____
 *  / _ \                          |  ___|      |  _  /  ___|
 * / /_\ \_   _ _ __ ___  _ __ __ _|___ \ ______| | | \ `--.
 * |  _  | | | | '__/ _ \__/ _ \| '__/ _`|\ \___| | | |`--. \
 * | | | | |_| | | | (_) | | | (_| /\__/ /      \ \_/ /\__/ /
 * \_| |_/\__,_|_|  \___/|_|  \__,_\____/        \___/\____/
 *
 * @name   : Aurora5-OS Shell
 * @date   : 13/09/2022
 * @authors: Nguyễn Văn Khánh Duy (2051067158)
 *         : Nguyễn Hoài Nguyệt An (2051067525)
 *         : Nguyễn Phạm Xuân Hiền (2051067165)
 *         : Nguyễn Phùng Hưng (2051067550)
 *         : Trần Văn Tân (2051067560)
 * @details: Dự án là phần thuộc bài tập lớn môn Nguyên Lý Hệ Điều Hành lớp S23-62TH
 *           Vui lòng ghi gõ nguồn tác giả nếu muốn sử dụng để tham khảo.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <string.h>
#include <sys/statvfs.h>
#include <signal.h>
#include <setjmp.h>

#include "libs/constants.h"

#include "libs/os/shell.h"
#include "libs/os/first_setup.h"
#include "libs/utils/common.h"
#include "libs/user/users_manager.h"
#include "libs/command/command_processer.h"

const char *OS_NAME = "Aurora5-OS";

const char *OS_VERSION = "1.1.0";

void sigintHandler(int sig_num);
UserInfo *login(char *root_dir);
int init_shell();

int main()
{
    // chống người dùng thoát
    signal(SIGINT, sigintHandler);

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
    UserInfo *user = login(ROOT_DIR);

    // khởi động Shell
    return init_shell(user, ROOT_DIR);
}

/**
 * Hàm đăng nhập vào Shell
 * @param root_dir: đường dẫn tuyệt đối từ máy chính đến Shell
 *
 */
UserInfo *login(char *root_dir)
{
    char *user_name;
    int _is_user_exist = 0;

    while (_is_user_exist == 0)
    {
        user_name = (char *)calloc(MAX_BUFFER_SIZE, sizeof(char));
        printf("Login: ");
        scanf("%s", user_name);
        _is_user_exist = is_user_exist(user_name, root_dir);

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
        _is_correct_pass = is_correct_password(user_name, password, root_dir);
        if (_is_correct_pass == 0)
        {
            printf("Incorrect password! Try again!\n");
        }
    }

    clear_c();

    // khởi tạo người dùng hiện tại
    char *CURRENT_USER = (char *)calloc(strlen(user_name) + 1, sizeof(char));

    strcpy(CURRENT_USER, user_name);

    // hostname
    char *HOST_NAME = get_host_name(root_dir);

    free(user_name);
    free(password);

    // giải phóng stdin
    fflush(stdin);
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    // khởi tạo thông tin shell
    UserInfo *user = malloc(sizeof(UserInfo));

    strcpy(user->current_user, CURRENT_USER);
    strcpy(user->host_name, get_host_name(root_dir));
    strcpy(user->root_dir, root_dir);
    strcpy(user->current_dir, get_user_dir(root_dir, CURRENT_USER));
    user->is_admin = (is_admin(CURRENT_USER, root_dir) == 1) ? 1 : 0;

    return user;
}

int init_shell(UserInfo *user, char *root_dir)
{
    // Nếu khởi tạo được người dùng thì bắt đầu bật Shell
    if (user != NULL)
    {

        // cấp phát bộ nhớ cho câu lệnh
        char *command = (char *)calloc(MAX_BUFFER_SIZE, sizeof(char));

        welcome_message(OS_VERSION);

        // khởi tạo exit code mặc định là 0
        int exit_code = SUCCESS_EXIT_CODE;

        add_user_to_shell(root_dir, *user);

        // shell sẽ chạy liên tục đến khi nào gặp exit_code là 128
        while (exit_code != EXIT_EXIT_CODE)
        {
            aurora_shell(user->current_user, user->host_name, user->root_dir, user->current_dir, exit_code);

            fgets(command, MAX_BUFFER_SIZE, stdin);

            exit_code = execute_command(command, root_dir);

            user = get_current_user(root_dir);
            if (user == NULL)
            {
                exit_code = EXIT_EXIT_CODE;
            }
        }

        free(command);
        return SUCCESS_EXIT_CODE;
    }

    return ERROR_EXIT_CODE;
}

void sigintHandler(int sig_num)
{
    /* Reset handler to catch SIGINT next time.
    Refer http://en.cppreference.com/w/c/program/signal */
    signal(SIGINT, sigintHandler);
    fflush(stdout);
}
