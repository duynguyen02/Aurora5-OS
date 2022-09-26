/**
 * Thư viện quản lý người dùng của Aurora5-OS
 * @author Nguyễn Văn Khánh Duy
 * @date 15/09/2022
 */

#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../constants.h"

#include "../utils/common.h"
#include "../security/security.h"

#include "users_manager.h"



char *create_passwd_file_path(const char *rootPath);
char *create_hostname_file_path(const char *rootPath);
int is_user_exist(char *userName, const char *rootPath);
int is_correct_password(char *userName, char *password, const char *rootPath);
int is_admin(char *userName, const char *rootPath);
int add_user(char *userName, char *password, int isAdmin, const char *rootPath);
int remove_user(char *userName, const char *rootPath);
char *get_host_name(const char *rootPath);
char *get_user_dir(const char *rootPath, const char *username);
int add_user_to_shell(const char *rootPath, UserInfo user);
int append_user_to_shell(const char *rootPath, UserInfo user);
UserInfo *get_current_user(const char *rootPath);
int replace_the_last_shell_user(const char *rootPath, UserInfo user);
int is_user_active(char *username, char *rootPath);



/**
 * tạo đường dẫn cho file passwd
 * @param rootPath: đường dẫn tuyệt đối từ máy chính tới Aurora5-OS
 * @return đường dẫn tuyệt đối tới tệp passwd
 */
char *create_passwd_file_path(const char *rootPath)
{
    // tạo đường dẫn tuyệt đối cho file passwd
    char *passwd_file_path;
    passwd_file_path = malloc(strlen(rootPath) + strlen(ETC_DIR) + strlen(PASSWD_FILE) + 1);
    strcpy(passwd_file_path, rootPath);
    strcat(passwd_file_path, ETC_DIR);
    strcat(passwd_file_path, PASSWD_FILE);
    return passwd_file_path;
}

/**
 * tạo đường dẫn cho file hostname
 * @param rootPath: đường dẫn tuyệt đối từ máy chính tới Aurora5-OS
 * @return đường dẫn tuyệt đối tới tệp hostname
 */
char *create_hostname_file_path(const char *rootPath)
{
    // tạo đường dẫn tuyệt đối cho file hostname
    char *passwd_file_path;
    passwd_file_path = malloc(strlen(rootPath) + strlen(ETC_DIR) + strlen(HOST_NAME_FILE) + 1);
    strcpy(passwd_file_path, rootPath);
    strcat(passwd_file_path, ETC_DIR);
    strcat(passwd_file_path, HOST_NAME_FILE);
    return passwd_file_path;
}

/**
 * Kiểm tra xem người dùng có tồn tại hay không
 * @param rootPath: đường dẫn tuyệt đối từ máy chính tới Aurora5-OS
 * @param userName: tên người dùng
 * @return 1 nếu người dùng tồn tại ngược lại là 0
 */
int is_user_exist(char *userName, const char *rootPath)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(create_passwd_file_path(rootPath), "r");
    if (fp == NULL)
        return 0;

    while ((read = getline(&line, &len, fp)) != -1)
    {
        char *token = strtok(line, ":");
        if (strcmp(userName, token) == 0)
        {
            return 1;
        }
    }

    fclose(fp);
    if (line)
        free(line);
    return 0;
}

/**
 * Kiểm tra xem người dùng có đúng hay không
 * @param rootPath: đường dẫn tuyệt đối từ máy chính tới Aurora5-OS
 * @param userName: tên người dùng
 * @param password: mật khẩu người dùng
 * @return 1 nếu mật khẩu chính xác ngược lại là 0
 */
int is_correct_password(char *userName, char *password, const char *rootPath)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(create_passwd_file_path(rootPath), "r");
    if (fp == NULL)
        return 0;

    while ((read = getline(&line, &len, fp)) != -1)
    {
        char *token = strtok(line, ":");
        if (strcmp(userName, token) == 0)
        {
            token = strtok(NULL, ":");
            if (strcmp(encrypt_sha512(password), token) == 0)
            {
                return 1;
            }
        }
    }

    fclose(fp);
    if (line)
        free(line);
    return 0;
}

/**
 * Kiểm tra xem người dùng có phải quản trị viên hay không
 * @param rootPath: đường dẫn tuyệt đối từ máy chính tới Aurora5-OS
 * @param userName: tên người dùng
 * @return 1 nếu người dùng là quản trị viên ngược lại là 0
 */
int is_admin(char *userName, const char *rootPath)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(create_passwd_file_path(rootPath), "r");
    if (fp == NULL)
        return 0;

    while ((read = getline(&line, &len, fp)) != -1)
    {
        char *token = strtok(line, ":");
        if (strcmp(userName, token) == 0)
        {
            token = strtok(NULL, ":");
            token = strtok(NULL, ":");
            if (strcmp(token, "1\n") == 0)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }

    fclose(fp);
    if (line)
        free(line);
    return 0;
}

/**
 * Thêm người dùng vào Aurora5-OS
 * @param rootPath: đường dẫn tuyệt đối từ máy chính tới Aurora5-OS
 * @param userName: tên người dùng
 * @param password: mật khẩu người dùng
 * @param isAdmin : người dùng có phải quản trị viên
 * @return 1 nếu thêm thành công ngược lại là 0
 */
int add_user(char *userName, char *password, int isAdmin, const char *rootPath)
{
    // tạo đường dẫn tuyệt đối cho file passwd
    char *passwd_file_path = create_passwd_file_path(rootPath);

    // nếu đường dẫn không tồn tại
    if (is_file_exists(passwd_file_path) == 0)
    {
        return 0;
    }

    if (is_user_exist(userName, rootPath) == 1)
    {
        return 0;
    }

    // tiến hành mở file và thêm user
    FILE *file_ptr = fopen(passwd_file_path, "a");
    if (file_ptr != NULL)
    {
        // mã hóa mật khẩu
        char *encrypt_pass = encrypt_sha512(password);
        char *isAdminStr = (isAdmin == 1) ? "1" : "0";

        /// tạo chuỗi được format để thêm vào file ///
        char *user_info = malloc(strlen(userName) + strlen(encrypt_pass) + 4);
        strcpy(user_info, userName);
        strcat(user_info, ":");
        strcat(user_info, encrypt_pass);
        strcat(user_info, ":");
        strcat(user_info, isAdminStr);
        strcat(user_info, "\n");
        fputs(user_info, file_ptr);
        fclose(file_ptr);
        char *user_path = calloc(MAX_BUFFER_SIZE, sizeof(char));
        strcpy(user_path, rootPath);
        strcat(user_path, HOME_DIR);
        strcat(user_path, "/");
        strcat(user_path, userName);
        if (strcmp(userName, ADMIN_USER_NAME) != 0)
        {
            mkdir(user_path, 0777);
        }
        return 1;
    }

    return 0;
}

/**
 * Xóa người dùng khỏi Aurora5-OS
 * @param rootPath: đường dẫn tuyệt đối từ máy chính tới Aurora5-OS
 * @param userName: tên người dùng
 * @return 1 nếu xóa thành công ngược lại là 0
 */
int remove_user(char *userName, const char *rootPath)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(create_passwd_file_path(rootPath), "r");
    if (fp == NULL)
        return 0;

    char * users_str = calloc(MAX_BUFFER_SIZE, sizeof(char));

    while ((read = getline(&line, &len, fp)) != -1)
    {
        char * temp_line = calloc(MAX_BUFFER_SIZE, sizeof(char));
        strcpy(temp_line, line);

        char *token = strtok(line, ":");
        if (strcmp(userName, token) != 0)
        {
            strcat(users_str, temp_line);
        }
    }

    fclose(fp);
    if (line)
        free(line);

    fp = fopen(create_passwd_file_path(rootPath), "w");
    fputs(users_str, fp);
    fclose(fp);

    return 1;
}

/**
 * lấy tên máy chủ trong file hostname
 * @param rootPath: đường dẫn tuyệt đối từ máy chính tới Aurora5-OS
 * @return tên của máy chủ
 */
char *get_host_name(const char *rootPath)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(create_hostname_file_path(rootPath), "r");
    if (fp == NULL)
        return "";

    while ((read = getline(&line, &len, fp)) != -1)
    {
        return line;
    }

    fclose(fp);
    if (line)
        free(line);
    return "";
}

/**
 * Lấy đường dẫn tuyệt đối của thư mục của người dùng
 * @param rootPath: đường dẫn tuyệt đối từ máy chính tới Aurora5-OS
 * @param username: tên người dùng
 * @return đường dẫn tuyệt đối của thư mục của người dùng
 */
char *get_user_dir(const char *rootPath, const char *username)
{
    char *user_path = calloc(MAX_BUFFER_SIZE, sizeof(char));

    if (strcmp(username, ADMIN_USER_NAME) == 0)
    {
        strcpy(user_path, rootPath);
        return user_path;
    }

    char *home_path = calloc(MAX_BUFFER_SIZE, sizeof(char));

    strcpy(home_path, HOME_DIR);
    strcat(home_path, "/");
    strcat(home_path, username);

    strcpy(user_path, rootPath);
    strcat(user_path, home_path);

    return user_path;
}

/**
 * Nạp người dùng đăng nhập đầu tiên vào shell
 * @param rootPath: đường dẫn tuyệt đối từ máy chính tới Aurora5-OS
 * @param user: cấu trúc chứa thông tin người dùng 
 * @return 1 nếu thêm thành công ngược lại là 0
 */
int add_user_to_shell(const char *rootPath, UserInfo user)
{
    char *user_shell_path = calloc(strlen(rootPath) + strlen(ETC_DIR) + strlen(USER_SHELL_FILE) + 1, sizeof(char));

    strcpy(user_shell_path, rootPath);
    strcat(user_shell_path, ETC_DIR);
    strcat(user_shell_path, USER_SHELL_FILE);

    UserInfo users[1];

    users[0] = user;

    FILE *file;
    file = fopen(user_shell_path, "w");

    if (file == NULL)
    {
        return 0;
    }

    fwrite(users, sizeof(UserInfo), 1, file);

    fclose(file);

    return 1;
}

/**
 * Nạp các người dùng đăng nhập tiếp theo vào shell
 * @param rootPath: đường dẫn tuyệt đối từ máy chính tới Aurora5-OS
 * @param user: cấu trúc chứa thông tin người dùng 
 * @return 1 nếu thêm thành công ngược lại là 0
 */
int append_user_to_shell(const char *rootPath, UserInfo user)
{
    char *user_shell_path = calloc(strlen(rootPath) + strlen(ETC_DIR) + strlen(USER_SHELL_FILE) + 1, sizeof(char));

    strcpy(user_shell_path, rootPath);
    strcat(user_shell_path, ETC_DIR);
    strcat(user_shell_path, USER_SHELL_FILE);

    UserInfo users[1];

    users[0] = user;

    FILE *file;
    file = fopen(user_shell_path, "ab");

    if (file == NULL)
    {
        return 0;
    }

    fwrite(users, sizeof(UserInfo), 1, file);

    fclose(file);

    return 1;
}

/**
 * Lây thông tin người dùng đang đăng nhập vào Shell
 * @param rootPath: đường dẫn tuyệt đối từ máy chính tới Aurora5-OS
 * @return địa chỉ trỏ đến cấu trúc thông tin của người dùng
 */
UserInfo *get_current_user(const char *rootPath)
{
    char *user_shell_path = calloc(strlen(rootPath) + strlen(ETC_DIR) + strlen(USER_SHELL_FILE) + 1, sizeof(char));

    strcpy(user_shell_path, rootPath);
    strcat(user_shell_path, ETC_DIR);
    strcat(user_shell_path, USER_SHELL_FILE);

    FILE *file;
    file = fopen(user_shell_path, "r");

    if (file == NULL)
    {
        return NULL;
    }

    unsigned int n_stud = 0;

    UserInfo users[MAX_OF_USER];

    while (fread(&users[n_stud], sizeof(UserInfo), 1, file) == 1)
    {
        n_stud++;
    }

    fclose(file);

    if (n_stud == 0)
    {
        return NULL;
    }

    UserInfo *l_user = &users[n_stud - 1];

    return l_user;
}

/**
 * Thay thế thông tin người dùng đang đăng nhập vào Shell
 * @param rootPath: đường dẫn tuyệt đối từ máy chính tới Aurora5-OS
 * @param user: cấu trúc người dùng chứa thông tin bị thay đổi
 * @return trả về 1 nếu thay thế thành công ngược lại là 0
 */
int replace_the_last_shell_user(const char *rootPath, UserInfo user)
{
    char *user_shell_path = calloc(strlen(rootPath) + strlen(ETC_DIR) + strlen(USER_SHELL_FILE) + 1, sizeof(char));

    strcpy(user_shell_path, rootPath);
    strcat(user_shell_path, ETC_DIR);
    strcat(user_shell_path, USER_SHELL_FILE);

    // đọc danh sách người dùng đang đăng nhập shell
    FILE *file;
    file = fopen(user_shell_path, "r");

    if (file == NULL)
    {
        return 0;
    }

    unsigned int n_stud = 0;

    UserInfo users[MAX_OF_USER];

    while (fread(&users[n_stud], sizeof(UserInfo), 1, file) == 1)
    {
        n_stud++;
    }

    fclose(file);

    // nếu không còn người thì trả về false
    if (n_stud == 0)
    {
        return 0;
    }

    // còn thì xóa người dùng theo cơ chế stack
    file = fopen(user_shell_path, "w");

    if (file == NULL)
    {
        return 0;
    }

    UserInfo renew_users[n_stud];
    for (int i = 0; i < n_stud; i++)
    {
        renew_users[i] = users[i];
    }
    renew_users[n_stud - 1] = user;
    fwrite(renew_users, sizeof(UserInfo), n_stud, file);

    fclose(file);
    return 1;
}

/**
 * Kiểm tra xem user có đang hoạt động trong shell hay không
 * @param username: tên người dùng
 * @param rootPath: đường dẫn tuyệt đối từ máy chính tới Aurora5-OS
 * @return trả về 1 nếu đang hoạt động ngược lại là 0
 */
int is_user_active(char *username, char *rootPath)
{
    char *user_shell_path = calloc(strlen(rootPath) + strlen(ETC_DIR) + strlen(USER_SHELL_FILE) + 1, sizeof(char));

    strcpy(user_shell_path, rootPath);
    strcat(user_shell_path, ETC_DIR);
    strcat(user_shell_path, USER_SHELL_FILE);

    // đọc danh sách người dùng đang đăng nhập shell
    FILE *file;
    file = fopen(user_shell_path, "r");

    if (file == NULL)
    {
        return 0;
    }

    unsigned int n_stud = 0;

    UserInfo users[MAX_OF_USER];

    while (fread(&users[n_stud], sizeof(UserInfo), 1, file) == 1)
    {
        n_stud++;
    }

    fclose(file);

    // nếu không còn người thì trả về false
    if (n_stud == 0)
    {
        return 0;
    }

    for (int i = 0; i<n_stud;i++){
        if (strcmp(username, users[i].current_user) == 0){
            return 1;
        }
    }

    return 0;

}
