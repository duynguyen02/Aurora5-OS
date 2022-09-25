/**
 * @author Trần Văn Tân
 * Câu lệnh xóa thư mục và tất cả nội dung trong thư mục
 */

#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <ftw.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "../../libs/constants.h"
#include "../../libs/utils/common.h"
#include "../../libs/utils/aucolors.h"
#include "../../libs/user/users_manager.h"

/**
 * hàm tham khảo từ: https://stackoverflow.com/questions/5467725/how-to-delete-a-directory-and-its-contents-in-posix-c
 */
int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    int rv = remove(fpath);

    if (rv)
        perror(fpath);

    return rv;
}

/**
 * hàm tham khảo từ: https://stackoverflow.com/questions/5467725/how-to-delete-a-directory-and-its-contents-in-posix-c
 */
int rmrf(char *path)
{
    return nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
}


/**
 * Hàm xóa thư mục
 */
int delete_folder(char *filename, char *rootPath)
{
    // lấy người dùng hiện hành
    UserInfo *user = get_current_user(rootPath);

    // nếu không lấy được sẽ báo lỗi
    if (user == NULL)
    {
        printf("Something went wrong, try again!\n");
        return ERROR_EXIT_CODE;
    }
    
    // cấp phát bộ nhớ cho chuỗi đường dẫn thư mục
    char *final_path = calloc(MAX_BUFFER_SIZE, sizeof(char));
    strcpy(final_path, user->current_dir);
    strcat(final_path, "/");
    strcat(final_path, filename);
    // nếu đường dẫn không tồn tại sẽ báo lỗi
    if (is_dir_exists(final_path) == 0)
    {
        printf("rmdir: can not create folder\n");
        printf("Folder does not exists\n");
        return NOT_FOUND_EXIT_CODE;
    }

    // tiến hành xóa thư mục
    rmrf(final_path);

    return SUCCESS_EXIT_CODE;
}

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        printf("rmdir: missing file operand\n");
        printf("Try 'rmdir --help' for more information.\n");
        return ERROR_EXIT_CODE;
    }

    if (argc > 3)
    {
        printf("rmdir: Error: to many arguments, try again!\n");
        return ERROR_EXIT_CODE;
    }

    if (strcmp(argv[1], "--help\n") == 0)
    {
        printf("Usage: rmdir [dirname]\n");
        printf("Delete a folder\n");
        return SUCCESS_EXIT_CODE;
    }
    else
    {
        if (argv[1][strlen(argv[1]) - 1] == 10)
        {
            argv[1][strlen(argv[1]) - 1] = '\0';
        }
        return delete_folder(argv[1], argv[argc - 1]);
    }
}