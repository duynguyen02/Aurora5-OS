/**
 * @author Nguyễn Văn Khánh Duy
 * @date 23/09/2022
 * Câu lệnh chuyển đổi thư mục hiện hành 
 */


#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "../../libs/constants.h"
#include "../../libs/utils/common.h"
#include "../../libs/user/users_manager.h"

// định nghĩa các hàm phụ trợ
char *get_parent_dir(const char *path);


// định nghĩa các hàm chính cho câu lệnh
int change_to_user_dir(const char *rootPath);
int change_to_root_dir(const char *rootPath);
int change_to_parent_dir(const char *rootPath);
int change_to_dir(char *rootPath, char *path);


int main(int argc, char **argv)
{
    // nếu tổng đối bằng 2 thì câu lệnh chỉ gồm cd và đường dẫn tới Shell: cd [root_path] 
    // thực hiện chuyển thư mục hiện hành về thư mục của người dùng hiện hành
    if (argc == 2)
    {
        return change_to_user_dir(argv[argc - 1]);
    }


    // nếu tổng đối số bằng 3 thì xét các trường hợp xảy ra
    if (argc == 3)
    {
        // nếu ký tự kết thúc của đối số thứ 2 chứa newline thì xóa đi
        if (argv[1][strlen(argv[1]) - 1] == 10)
        {
            argv[1][strlen(argv[1]) - 1] = '\0';
        }

        // nếu đối số thứ 2 là ~ thì chuyển về thư mục của người dùng hiện hành
        if (strcmp(argv[1], "~") == 0)
        {
            return change_to_user_dir(argv[argc - 1]);
        }
        // nếu đối số thứ 2 là / thì chuyển về thư mục của root
        else if (strcmp(argv[1], "/") == 0)
        {
            return change_to_root_dir(argv[argc - 1]);
        }
        // nếu đối số thứ 2 là .. thì chuyển về thư mục cha của thư mục hiện hành
        else if (strcmp(argv[1], "..") == 0)
        {
            return change_to_parent_dir(argv[argc - 1]);
        }
        // nếu đối số thứ 2 là . thì không chuyển vào thư mục nào cả
        else if (strcmp(argv[1], ".") == 0)
        {
            return SUCESS_EXIT_STATUS;
        }
        // còn lại thì sẽ vào hàm change_to_dir để xử lý
        else
        {
            return change_to_dir(argv[argc - 1], argv[1]);
        }
    }

    // thông báo lỗi nếu đối số có vấn đề
    printf("cd: Error: too many or few arguments, try again!\n");
    return ERROR_EXIT_STATUS;
}

/**
 * Hàm chuyển đường dẫn hiện hành về thư mục người dùng
 * @param rootPath: đường dẫn tuyệt đối từ máy chính đến Aurora5-OS
 * @return trạng thái thực thi được định nghĩa tại "../../libs/constants.h"
 */
int change_to_user_dir(const char *rootPath)
{
    // lấy người dùng đang đăng nhập vào shell
    UserInfo *user = get_current_user(rootPath);

    // nếu không có người dùng nào thì thoát shell
    if (user == NULL)
    {
        printf("Shell do not have any user!\n");
        return EXIT_EXIT_CODE;
    }

    // nếu người dùng là root
    if (strcmp(user->current_user, ADMIN_USER_NAME) == 0)
    {
        // thì chuyển đường dẫn được trỏ tới của người dùng đang đăng nhập thành rootPath
        strcpy(user->current_dir, rootPath);
    }
    // nếu là người dùng khác root
    else
    {
        // thì chuyển đường dẫn được trỏ tới của người dùng đang đăng nhập thành 
        // đường dẫn dẫn tới thư mục của người dùng đó được lấy thông qua gọi hàm
        // get_user_dir thuộc 'Aurora5-OS API'
        strcpy(user->current_dir, get_user_dir(rootPath, user->current_user));
    }

    // tiến hành thay đổi thông tin đã thay đổi thư mục trỏ tới của người dùng 
    // đang đăng nhập vào shell thông qua gọi hàm replace_the_last_shell_user
    // thuộc 'Aurora5-OS API'
    // nếu không thành công sẽ trả về lỗi
    if (replace_the_last_shell_user(rootPath, *user) == 0)
    {
        printf("Something went wrong, try again!\n");
        return ERROR_EXIT_CODE;
    }

    // trả về trạng thái thành công
    return SUCCESS_EXIT_CODE;
}

/**
 * Hàm chuyển đường dẫn hiện hành về thư mục root
 * @param rootPath: đường dẫn tuyệt đối từ máy chính đến Aurora5-OS
 * @return trạng thái thực thi được định nghĩa tại "../../libs/constants.h"
 */
int change_to_root_dir(const char *rootPath)
{
    // lấy người dùng đang đăng nhập vào shell
    UserInfo *user = get_current_user(rootPath);

    // nếu không có người dùng nào thì thoát shell
    if (user == NULL)
    {
        printf("Shell do not have any user!\n");
        return EXIT_EXIT_CODE;
    }
    // chuyển đường dẫn được trỏ tới của người dùng đang đăng nhập thành rootPath
    strcpy(user->current_dir, rootPath);

    // tiến hành thay đổi thông tin đã thay đổi thư mục trỏ tới của người dùng 
    // đang đăng nhập vào shell thông qua gọi hàm replace_the_last_shell_user
    // thuộc 'Aurora5-OS API'
    // nếu không thành công sẽ trả về lỗi    
    if (replace_the_last_shell_user(rootPath, *user) == 0)
    {
        printf("Something went wrong, try again!\n");
        return ERROR_EXIT_CODE;
    }

    // trả về trạng thái thành công
    return SUCCESS_EXIT_CODE;
}

/**
 * Hàm lấy đường dẫn cha của đường dẫn được truyền vào
 * @param path: đường dẫn cần lấy đường dẫn cha
 * @return chuỗi chứa đường dẫn cha, nếu không lấy được sẽ trả về chuỗi rỗng
 */
char *get_parent_dir(const char *path)
{
    // lấy kí tự '/' cuối cùng của chuỗi
    // gắn cờ lệnh là -1 tức không lấy được
    // duyệt từng ký tự trong chuỗi, nếu xuất hiện thì đổi cờ lệnh 
    // thành vị trí chứa '/' cuối cùng
    int index = -1;
    for (int i = strlen(path) - 1; i >= 0; i--)
    {
        if (path[i] == '/')
        {
            index = i;
            break;
        }
    }

    // nếu cờ lệnh không đổi sẽ trả chuỗi rỗng
    if (index == -1)
    {
        return "";
    }

    // tiến hành cắt chuỗi con
    char *temp_path = calloc(MAX_BUFFER_SIZE, sizeof(char));
    strcpy(temp_path, path);
    char *start = &temp_path[0];
    char *end = &temp_path[index];
    char *substr = (char *)calloc(1, end - start + 1);
    memcpy(substr, start, end - start);

    // trả chuỗi về chỗ gọi
    return substr;
}

/**
 * Hàm chuyển đường dẫn hiện hành về thư mục cha
 * @param rootPath: đường dẫn tuyệt đối từ máy chính đến Aurora5-OS
 * @return trạng thái thực thi được định nghĩa tại "../../libs/constants.h"
 */
int change_to_parent_dir(const char *rootPath)
{
    // lấy người dùng đang đăng nhập vào shell
    UserInfo *user = get_current_user(rootPath);
    // nếu không có người dùng nào thì thoát shell
    if (user == NULL)
    {
        printf("Shell do not have any user!\n");
        return EXIT_EXIT_CODE;
    }

    // nếu thư mục hiện tại khác như mục gốc thì lấy thư mục cha
    if (strcmp(user->current_dir, rootPath) != 0)
    {
        // nếu hàm get_parent_dir trả về rỗng thì báo lỗi
        if (strcmp(get_parent_dir(user->current_dir), "") == 0)
        {
            printf("Something went wrong, try again!\n");
            return ERROR_EXIT_CODE;
        }

        // không thì gán đường dẫn trỏ tới của người dùng thành 
        // đường dẫ tới thư mục cha
        strcpy(user->current_dir, get_parent_dir(user->current_dir));
    }

    // tiến hành thay đổi thông tin đã thay đổi thư mục trỏ tới của người dùng 
    // đang đăng nhập vào shell thông qua gọi hàm replace_the_last_shell_user
    // thuộc 'Aurora5-OS API'
    // nếu không thành công sẽ trả về lỗi   
    if (replace_the_last_shell_user(rootPath, *user) == 0)
    {
        printf("Something went wrong, try again!\n");
        return ERROR_EXIT_CODE;
    }

    // trả về trạng thái thành công
    return SUCCESS_EXIT_CODE;
}

/**
 * Hàm chuyển đường dẫn hiện hành đến các thư mục con
 * @param rootPath: đường dẫn tuyệt đối từ máy chính đến Aurora5-OS
 * @param path: đường dẫn cần chuyển tới
 * @return trạng thái thực thi được định nghĩa tại "../../libs/constants.h"
 */
int change_to_dir(char *rootPath, char *path)
{
    // cấp phát bộ nhớ cho biến chứa đường dẫn sẽ được người dùng trỏ tới
    char *final_path = calloc(MAX_BUFFER_SIZE, sizeof(char));

    // lấy người dùng đang đăng nhập vào shell
    UserInfo *user = get_current_user(rootPath);
    // nếu không có người dùng nào thì thoát shell
    if (user == NULL)
    {
        printf("Shell do not have any user!\n");
        return EXIT_EXIT_CODE;
    }

    // nếu ký tự đầu tiền là '.' cd sẽ thực hiện đến các thư mục con của thư mục hiện tại
    if (path[0] == '.')
    {
        // gán thư mục hiện tại vào biến final_path
        strcpy(final_path, user->current_dir);

        // tạo một biến tạm để chứ đường dẫn người dùng cần trỏ tới
        char *temp_path = calloc(MAX_BUFFER_SIZE, sizeof(char));
        strcpy(temp_path, path);

        // nếu sau '.' là '/'
        if (path[1] == '/')
        {
            // nếu sau kí tự / còn ký tự tức là user muốn vào thư mục con
            // tiến hành cắt từ sau '/' đến hết vào biến final_path
            if (path[2] != '\0')
            {
                char *start = &temp_path[1];
                char *end = &temp_path[strlen(temp_path)];

                char *substr = (char *)calloc(1, end - start + 1);

                memcpy(substr, start, end - start);

                strcat(final_path, substr);
            }

            // nếu thư mục không tồn tại thì báo lỗi
            if (is_dir_exists(final_path) == 0)
            {
                printf("cd: no such file or directory\n");
                return NOT_FOUND_EXIT_CODE;
            }

            
            strcpy(user->current_dir, final_path);

            if (replace_the_last_shell_user(rootPath, *user) == 0)
            {
                printf("Something went wrong, try again!\n");
                return ERROR_EXIT_CODE;
            }

            return SUCCESS_EXIT_CODE;
        }
        else
        {
            strcat(final_path, "/");

            char *start = &temp_path[0];
            char *end = &temp_path[strlen(temp_path)];

            char *substr = (char *)calloc(1, end - start + 1);

            memcpy(substr, start, end - start);

            strcat(final_path, substr);

            // nếu thư mục không tồn tại thì báo lỗi
            if (is_dir_exists(final_path) == 0)
            {
                printf("cd: no such file or directory\n");
                return NOT_FOUND_EXIT_CODE;
            }

            // không thì gán đường dẫn trỏ tới của người dùng thoe biến final_path
            strcpy(user->current_dir, final_path);


            // tiến hành thay đổi thông tin đã thay đổi thư mục trỏ tới của người dùng 
            // đang đăng nhập vào shell thông qua gọi hàm replace_the_last_shell_user
            // thuộc 'Aurora5-OS API'
            // nếu không thành công sẽ trả về lỗi  
            if (replace_the_last_shell_user(rootPath, *user) == 0)
            {
                printf("Something went wrong, try again!\n");
                return ERROR_EXIT_CODE;
            }

            return SUCCESS_EXIT_CODE;
        }
    }
    // nếu ký tự đầu tiền là '~' cd sẽ thực hiện đến các thư mục con của thư mục người dùng
    else if (path[0] == '~')
    {
        strcpy(final_path, get_user_dir(rootPath, user->current_user));

        if (path[1] == '/')
        {
            // nếu sau kí tự / không còn kí tự sẽ chuyển về thư mục người dùng.
            if (path[2] == '\0')
            {
                return change_to_user_dir(rootPath);
            }

            char *temp_path = calloc(MAX_BUFFER_SIZE, sizeof(char));
            strcpy(temp_path, path);

            strcat(final_path, "/");

            char *start = &temp_path[2];
            char *end = &temp_path[strlen(temp_path)];

            char *substr = (char *)calloc(1, end - start + 1);

            memcpy(substr, start, end - start);

            strcat(final_path, substr);

            // nếu thư mục không tồn tại thì báo lỗi
            if (is_dir_exists(final_path) == 0)
            {
                printf("cd: no such file or directory\n");
                return NOT_FOUND_EXIT_CODE;
            }

            strcpy(user->current_dir, final_path);

            if (replace_the_last_shell_user(rootPath, *user) == 0)
            {
                printf("Something went wrong, try again!\n");
                return ERROR_EXIT_CODE;
            }

            return SUCCESS_EXIT_CODE;
        }
        else
        {
            printf("Error location, try again!\n");
            return NOT_FOUND_EXIT_CODE;
        }
    }

    // nếu ký tự đầu tiền là '/' cd sẽ thực hiện đến các thư mục con của thư mục root
    else if (path[0] == '/')
    {
        strcpy(final_path, rootPath);

        if (path[1] != '/')
        {

            char *temp_path = calloc(MAX_BUFFER_SIZE, sizeof(char));
            strcpy(temp_path, path);

            strcat(final_path, "/");

            char *start = &temp_path[1];
            char *end = &temp_path[strlen(temp_path)];

            char *substr = (char *)calloc(1, end - start + 1);

            memcpy(substr, start, end - start);

            strcat(final_path, substr);

            // nếu thư mục không tồn tại thì báo lỗi
            if (is_dir_exists(final_path) == 0)
            {
                printf("cd: no such file or directory\n");
                return NOT_FOUND_EXIT_CODE;
            }

            strcpy(user->current_dir, final_path);

            if (replace_the_last_shell_user(rootPath, *user) == 0)
            {
                printf("Something went wrong, try again!\n");
                return ERROR_EXIT_CODE;
            }

            return SUCCESS_EXIT_CODE;
        }
        else
        {
            printf("Error location, try again!\n");
            return NOT_FOUND_EXIT_CODE;
        }
    }
    // trường hợp còn lại thì cũng thử trỏ đến thư mục con của thư mục hiện hành
    else
    {
        strcpy(final_path, user->current_dir);
        strcat(final_path, "/");
        strcat(final_path, path);

        // nếu thư mục không tồn tại thì báo lỗi
        if (is_dir_exists(final_path) == 0)
        {
            printf("cd: no such file or directory\n");
            return NOT_FOUND_EXIT_CODE;
        }

        strcpy(user->current_dir, final_path);

        if (replace_the_last_shell_user(rootPath, *user) == 0)
        {
            printf("Something went wrong, try again!\n");
            return ERROR_EXIT_CODE;
        }

        return SUCCESS_EXIT_CODE;
    }

    printf("Something went wrong, try again!\n");
    return ERROR_EXIT_CODE;
}