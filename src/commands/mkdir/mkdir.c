/**
 * @author Nguyễn Phùng Hưng
 * Câu lệnh tạo thư mục rỗng
 */

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <sys/stat.h>
#include <sys/types.h>

#include "../../libs/constants.h"
#include "../../libs/utils/common.h"
#include "../../libs/utils/aucolors.h"
#include "../../libs/user/users_manager.h"

/**
 * Hàm tạo folder
 */
int create_folder(char *filename, char *rootPath)
{

    // lấy người dùng hiện tại
    UserInfo *user = get_current_user(rootPath);

    // nếu không láy được sẽ báo lỗi
    if (user == NULL)
    {
        printf("Something went wrong, try again!\n");
        return ERROR_EXIT_CODE;
    }

    // tạo đường dẫn tuyệt đối cho thư mục
    char *final_path = calloc(MAX_BUFFER_SIZE, sizeof(char));
    strcpy(final_path, user->current_dir);
    strcat(final_path, "/");
    strcat(final_path, filename);

    // nếu thư mục đã tồn tại thì báo lỗi
    if (is_dir_exists(final_path) == 1)
    {
        printf("mkdir: can not create folder\n");
        printf("Folder exists\n");
        return ERROR_EXIT_CODE;
    }
    // tạo thư mục bằng Linux API
    mkdir(final_path, 0777);

    return SUCCESS_EXIT_CODE;
}

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        printf("mkdir: missing file operand\n");
        printf("Try 'mkdir --help' for more information.\n");
        return ERROR_EXIT_CODE;
    }

    if (argc > 3)
    {
        printf("mkdir: Error: to many arguments, try again!\n");
        return MISUSE_EXIT_CODE;
    }

    if (strcmp(argv[1], "--help\n") == 0)
    {
        printf("Usage: mkdir [dirname]\n");
        printf("Create a folder\n");
        return SUCCESS_EXIT_CODE;
    }
    else
    {
        if (argv[1][strlen(argv[1]) - 1] == 10)
        {
            argv[1][strlen(argv[1]) - 1] = '\0';
        }
        return create_folder(argv[1], argv[argc - 1]);
    }
}