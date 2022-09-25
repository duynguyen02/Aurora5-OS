/**
 * @author Nguyễn Phạm Xuân Hiện
 * Câu lệnh hiển thị các thư mục và tệp ở đường dẫn hiện hành
 */

#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "../../libs/constants.h"
#include "../../libs/utils/common.h"
#include "../../libs/utils/aucolors.h"



#include "../../libs/user/users_manager.h"

/**
 * Hàm hiển thị các thư mục và tệp tin hiện hành
 */
int list_current_dir(char *rootPath)
{

    // lấy thông tin người dùng hiện hành
    UserInfo *user = get_current_user(rootPath);

    // nếu người dùng khác rỗng thì thực hiện liệt kê
    if (user != NULL)
    {
        // trỏ tới thư mục hiện hành
        DIR *direntory;
        struct dirent *entry;
        direntory = opendir(user->current_dir);

        // nếu trỏ thất bại thì thông báo
        if (direntory == NULL)
        {
            // char *start = &user->current_dir[strlen(rootPath)];
            // char *end = &user->current_dir[strlen(user->current_dir)];

            // char * final_current_dir = (char*)calloc(255,sizeof(char));

            // memcpy(final_current_dir, start, end-start);

            printf("ls: cannot access '.': Error opening directory.\n");
            return NOT_FOUND_EXIT_CODE;
        }
        
        // nếu thành công thì bắt đầu liệt kê
        while ((entry = readdir(direntory)) != NULL)
        {
            // nếu tệp tin thì in màu trắng
            if (entry->d_type == DT_REG)
            {
                printf("%s\n", entry->d_name);
            }
            // nếu thư mục thì in màu xanh
            else if (entry->d_type == DT_DIR)
            {
                printf("%s%s%s\n", BLU,entry->d_name, RESET);
            }
        }

        printf("\n");

        closedir(direntory);

        return SUCCESS_EXIT_CODE;
    }
    return ERROR_EXIT_STATUS;
}


int main(int argc, char **argv)
{
    if (argc == 2)
    {
        return list_current_dir(argv[1]);
    }

    printf("Something went wrong, try again!");

    return ERROR_EXIT_CODE;
}