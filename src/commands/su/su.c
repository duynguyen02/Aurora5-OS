/**
 * @author Nguyễn Hoài Nguyệt An
 * Câu lệnh đăng nhập vào shell
 */

#include "stdio.h"
#include "string.h"
#include <unistd.h>
#include <stdlib.h>

#include "../../libs/constants.h"
#include "../../libs/utils/common.h"
#include "../../libs/utils/aucolors.h"
#include "../../libs/user/users_manager.h"

/**
 * Hàm thử đăng nhập
 */
int try_to_login(char * username, char * rootPath){

    // loại bỏ \n khỏi chuỗi username
    username[strlen(username)-1] = '\0';

    // @bug dữ liệu bị mất không lý do vì vậy
    // tạo một hàm tạm để chứa username và rootpath
    char * CURRENT_USER = calloc(MAX_BUFFER_SIZE, sizeof(char));
    strcpy(CURRENT_USER, username);
    char * ROOT_PATH = calloc(MAX_BUFFER_SIZE, sizeof(char));
    strcpy(ROOT_PATH, rootPath);

    // nếu người dùng không tồn tại thì trả mã lỗi
    if (is_user_exist(username, rootPath) == 0){
        printf("User does not exist.\n");
        return NOT_FOUND_EXIT_CODE;
    }

    // cấp phát bộ nhớ và yêu cầu người dùng nhập mật khẩu
    char *password = (char *)calloc(MAX_BUFFER_SIZE, sizeof(char));
    printf("Enter your password: ");
    password = strdup(getpass(""));

    // nếu password không chính xác thì báo lỗi
    if (is_correct_password(username, password, rootPath) == 0){
        printf("su: Authentication failure\n");
        return ERROR_EXIT_CODE;
    }

    // cấp phát bộ nhớ cho cấu trúc UserInfo
    UserInfo *user = malloc(sizeof(UserInfo));

    // đưa thông tin người dùng đang chuẩn bị đăng nhập vào cấu trúc
    strcpy(user->current_user, CURRENT_USER);
    strcpy(user->host_name, get_host_name(ROOT_PATH));
    strcpy(user->root_dir, ROOT_PATH);
    strcpy(user->current_dir, get_user_dir(ROOT_PATH, CURRENT_USER));
    user->is_admin = (is_admin(CURRENT_USER, ROOT_PATH) == 1) ? 1 : 0;

    // thêm vào shell, nếu không thành công thì thông báo lỗi
    if (append_user_to_shell(rootPath, *user) == 0){
        printf("Something went wrong, try again!\n");
        return ERROR_EXIT_CODE;
    }

    return SUCCESS_EXIT_CODE;

}

int main(int argc, char **argv)
{

    if (argc == 2)
    {
        printf("Try 'su --help' for more infomation.\n");
        return ERROR_EXIT_CODE;
    }

    if (argc == 3)
    {
        if (strcmp(argv[1], "--help\n") == 0)
        {
            printf("Usage: su [username]\n");
            printf("Login to another users\n");
            return SUCCESS_EXIT_CODE;
        }
        else
        {
            return try_to_login(argv[1], argv[argc-1]);
        }
    }


    printf("su: Error: to many arguments, try again!\n");

    return ERROR_EXIT_CODE;
}