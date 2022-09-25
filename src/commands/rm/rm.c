/**
 * @author Trần Văn Tân
 * Câu lệnh xóa tệp tin
 */

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "../../libs/constants.h"
#include "../../libs/utils/common.h"
#include "../../libs/utils/aucolors.h"
#include "../../libs/user/users_manager.h"


/**
 * Hàm xóa tệp tin
 */
int delete_file(char *filename, char *rootPath)
{

    // lấy nguời dùng hiện hành
    UserInfo *user = get_current_user(rootPath);

    // nếu không lấy được thì thông báo lỗi
    if (user == NULL)
    {
        printf("Something went wrong, try again!\n");
        return ERROR_EXIT_CODE;
    }

    // cập phát bộ nhớ và tạo đường dẫn tới tệp tin
    char *final_path = calloc(MAX_BUFFER_SIZE, sizeof(char));
    strcpy(final_path, user->current_dir);
    strcat(final_path, "/");
    strcat(final_path, filename);

    // nếu tệp tin không tồn tại thì báo lỗi
    if (is_file_exists(final_path) == 0)
    {
        printf("touch: can not delete file\n");
        printf("File does not exist\n");
        return NOT_FOUND_EXIT_CODE;
    }

    // sử dụng Linux API để xóa tệp tin nếu xóa không được sẽ báo lỗi
    if (remove(final_path) != 0){
        printf("Something went wrong, try again!\n");
        return ERROR_EXIT_CODE;
    }

    return SUCCESS_EXIT_CODE;
}

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        printf("rm: missing file operand\n");
        printf("Try 'rm --help' for more information.\n");
        return ERROR_EXIT_CODE;
    }

    if (argc > 3)
    {
        printf("touch: Error: to many arguments, try again!\n");
        return MISUSE_EXIT_CODE;
    }

    if (strcmp(argv[1], "--help\n") == 0)
    {
        printf("Usage: rm [filename]\n");
        printf("Delete a file\n");
        return SUCCESS_EXIT_CODE;
    }
    else
    {
        if (argv[1][strlen(argv[1]) - 1] == 10)
        {
            argv[1][strlen(argv[1]) - 1] = '\0';
        }
        return delete_file(argv[1], argv[argc - 1]);
    }
}