/**
 * @author Nguyễn Hoài Nguyệt An
 * Câu lệnh thực hiện chức năng thêm người dùng vào Aurora5-OS
 */

#include "string.h"
#include "../../libs/constants.h"
#include "../../libs/utils/common.h"
#include "../../libs/utils/aucolors.h"
#include "../../libs/user/users_manager.h"

/**
 * Thử thêm người dùng vào Aurora5-OS
 * @return các EXIT_CODE được định nghĩa trong constants.h
 */
int try_to_add_user(char *username, char *password, char *is_admin, char *rootPath)
{

    // nếu tên người dùng chứa dấu ':' thì không được chấp nhận
    if (strchr(username, ':') != NULL)
    {
        printf("':' symbol is not allowed\n");
        return ERROR_EXIT_CODE;
    }

    // nếu người dùng đã tồn tại thì không được thêm
    if (is_user_exist(username, rootPath) == 1)
    {
        printf("User: %s exists!\n", username);
        return NOT_FOUND_EXIT_CODE;
    }

    // do lỗi không xác định khi đặt điều kiện hoặc (||) 
    // nên gắn cờ lệnh để kiểm tra xem ký tự để dặt vào trò có hợp lệ hay không

    // lúc đâu cờ lệnh được gắn hợp lệ
    int is_invalid_key = 1;
    
    // kiểm tra ký tự đầu của chuỗi is_admin có phải là 'y'
    // nếu không phải thì đặt cờ lệnh là 0 tức không hợp lệ 
    if ((is_admin[0] != 121))
    {
        is_invalid_key = 0;
    }

    // nếu cờ lệnh không hợp lệ
    // kiểm tra xem ký tự đầu của chuỗi is_admin có phải là 'n'
    // nếu tiếp tục không hợp lệ thì thông báo cho người dùng và trả mã lỗi
    if (is_invalid_key == 0)
    {
        if ((is_admin[0] != 110))
        {
            printf("Invalid key '%c'", is_admin[0]);
            return ERROR_EXIT_CODE;
        }
    }

    // tiến hành thêm người dùng, nếu thành công thì trả mã thành công
    // nếu không trả mã lỗi
    if (add_user(
            username,
            password,
            (is_admin[0] == 121) ? 1 : 0,
            rootPath) == 1)
    {
        printf("Successful!\n");
        return SUCCESS_EXIT_CODE;
    }
    else
    {
        printf("Something went wrong, try again!\n");
        return ERROR_EXIT_CODE;
    }
}

int main(int argc, char **argv)
{
    // nếu tổng số đối số dòng lệnh bằng 2 
    // yêu cầu người dùng sử dụng useradd --help để xem hướng dẫn sử dụng
    if (argc == 2)
    {
        printf("Try 'useradd --help' for more infomation.\n");
        return ERROR_EXIT_CODE;
    }

    // nếu tổng số đối số dòng lệnh bằng 3
    // hiện thị hướng dẫn sử dụng nến đối số thứ 2 là --help
    // ngược lại thông báo và trả về mã lỗi
    if (argc == 3)
    {
        if (strcmp(argv[1], "--help\n") == 0)
        {
            printf("Usage: useradd [username] [password] [is admin?(y/n)]\n");
            return SUCCESS_EXIT_CODE;
        }
        else
        {
            printf("useradd: invalid option -- '%s'\n", argv[1]);
            return ERROR_EXIT_CODE;
        }
    }

    // nếu đối số là 5 thì kiểm tra xem đây có phải admin
    // nếu phải thì cho phép tiến hành thêm người dùng
    // ngược lại thông báo và trả mã lỗi
    if (argc == 5)
    {
        UserInfo *user = get_current_user(argv[4]);

        if (user->is_admin != 1)
        {
            printf("You are not root user to use this command!\n");
            return MISUSE_EXIT_CODE;
        }

        return try_to_add_user(argv[1], argv[2], argv[3], argv[4]);
    }

    // trả mã lỗi ở 1 số vấn đề phát sinh
    printf("useradd: Error: too many or few arguments, try again!\n");

    return ERROR_EXIT_CODE;
}