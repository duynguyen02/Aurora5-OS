/**
 * @author Nguyễn Hoài Nguyệt An
 * Câu lệnh thực hiện chức năng xóa người dùng khỏi Aurora5-OS
 */

#include "stdio.h"
#include "string.h"

#include "../../libs/constants.h"
#include "../../libs/utils/common.h"
#include "../../libs/utils/aucolors.h"
#include "../../libs/user/users_manager.h"

/**
 * Hàm xóa người dùng khỏi Aurora5-OS
 */
int delete_user(char * username, char * rootPath){

    // lấy người dùng đang đăng nhập vào shell
    UserInfo *user = get_current_user(rootPath);

    // nếu người bị xóa là root thì thông báo và trả mã lỗi
    if (strcmp(username, ADMIN_USER_NAME) == 0){
        printf("You can not delete root user!\n");
        return MISUSE_EXIT_CODE;
    }

    // nếu người dùng không tồn tại thì thông báo và trả mã lỗi
    if (is_user_exist(username, rootPath) == 0){
        printf("User: %s does not exist!\n", username);
        return NOT_FOUND_EXIT_CODE;
    }

    // kiểm tra xem có phải admin nếu không phải thì trả mã lỗi
    if (is_admin(user->current_user, rootPath) == 0){
        printf("You are not a root user to use this command!\n");
        return MISUSE_EXIT_CODE;
    }

    // nếu người dùng đăng nhập vào shell thì không được xóa
    if (is_user_active(username, rootPath) == 1){
        printf("You can not detele '%s', This user is logging on the shell.!\n", username);
        return MISUSE_EXIT_CODE;
    }

    // tiến hành xóa người dùng nếu không thành công thì thông báo và trả mã lỗi
    if (remove_user(username, rootPath) == 0){
        printf("Something went wrong, try again!\n");
        return ERROR_EXIT_CODE;
    }

    // nếu thành công thì thông báo và trả mã thành công
    printf("Successful!\n");
    return SUCCESS_EXIT_CODE;
}

int main(int argc, char **argv)
{
    // nếu đối số dòng lệnh là 2 thì gợi ý người dùng sử dụng userdel --help
    // để xem cách sử dụng
    if (argc == 2)
    {
        printf("Try 'userdel --help' for more infomation.\n");
        return ERROR_EXIT_CODE;
    }

    // nếu đối số bằng 3 thì kiếm tra nếu đối số thứ 2 là --help
    // thì hiển thị hướng dẫn sử dụng còn không tiến hành xóa người dùng
    if (argc == 3)
    {
        if (strcmp(argv[1], "--help\n") == 0)
        {
            printf("Usage: userdel [username]\n");
            printf("Delete a user\n");
            return SUCCESS_EXIT_CODE;
        }
        else
        {
            if (argv[1][strlen(argv[1]) - 1] == 10){
                argv[1][strlen(argv[1]) - 1] = '\0';
            }
            return delete_user(argv[1], argv[argc-1]);
        }
    }

    // thông báo và trả mã lỗi nếu có đối số không hợp lệ phát sinh
    printf("userdel: Error: to many or few arguments, try again!\n");

    return ERROR_EXIT_CODE;
}