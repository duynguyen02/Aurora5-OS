/**
 * @author Nguyễn Phùng Hưng
 * Câu lệnh tạo một tệp tin rỗng
 */

#include "stdio.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "../../libs/constants.h"
#include "../../libs/utils/common.h"
#include "../../libs/utils/aucolors.h"
#include "../../libs/user/users_manager.h"

/**
 * Hàm tạo file
 */
int create_file(char *filename, char *rootPath)
{
    // lấy người dùng hiện hành
    UserInfo *user = get_current_user(rootPath);

    // nếu không có người dùng thì báo lỗi
    if (user == NULL){
        printf("Something went wrong, try again!\n");
        return ERROR_EXIT_CODE;
    }

    // cấp phát bộ nhớ để tạo đường dẫn tuyệt đối cho tệp tin
    char * final_path = calloc(MAX_BUFFER_SIZE, sizeof(char));
    strcpy(final_path, user->current_dir);
    strcat(final_path, "/");
    strcat(final_path, filename);

    // kiểm tra nếu tệp tin 
    if (is_file_exists(final_path) == 1){
        printf("touch: can not create file\n");
        printf("File exists\n");
        return MISUSE_EXIT_CODE;
    }

    // tạo một tệp tin rỗng
    FILE *fp;
    fp  = fopen (final_path, "w");
    fclose(fp);
    return SUCCESS_EXIT_CODE;
}

int main(int argc, char **argv)
{
    // nếu đối số bằng 2 thì gợi ý cách sử dụng
    if (argc == 2)
    {
        printf("touch: missing file operand\n");
        printf("Try 'touch --help' for more information.\n");
        return ERROR_EXIT_CODE;
    }

    // nếu đối số lớn hơn 3 thì thông báo lỗi
    if (argc > 3)
    {
        printf("touch: Error: to many arguments, try again!\n");
        return ERROR_EXIT_CODE;
    }

    // nếu đối số thứ 2 là --help thì hiển thị hướng dẫn sử dụng
    // còn không thì tiên hành tạo file
    if (strcmp(argv[1], "--help\n") == 0)
    {
        printf("Usage: touch [filename]\n");
        return SUCCESS_EXIT_CODE;
    }
    else
    {
        if (argv[1][strlen(argv[1]) - 1] == 10)
        {
            argv[1][strlen(argv[1]) - 1] = '\0';
        }
        return create_file(argv[1], argv[argc - 1]);
    }
}