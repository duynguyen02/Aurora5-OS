/**
 * Thư viện cài đặt các cấu hình cần thiết cho Aurora5-OS
 * @author Nguyễn Văn Khánh Duy
 * @date 15/09/2022
 */

#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "../constants.h"

#include "../utils/aucolors.h"
#include "../utils/common.h"
#include "../user/users_manager.h"
#include "shell.h"

void display_progess_bar(const char* osVersion);
void first_time_setup(const char * osName, const char * osVersion, const char * rootPath);


/**
 * Hàm cài đặt các thiết lập lần đầu cho Aurora5-OS
 */
void first_time_setup(const char * osName, const char * osVersion, const char * rootPath){

    char * etc_absolute_path;
    etc_absolute_path = malloc(strlen(rootPath)+strlen(ETC_DIR)+1); 
    strcpy(etc_absolute_path, rootPath);
    strcat(etc_absolute_path, ETC_DIR);

    text_color(GRN);
    printf("Setting Up Your %s For The First Time\n", osName);
    text_color(RESET);

    char *computer_name = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char));
    
    printf("Your computer name: ");
    scanf("%[^\n]s",computer_name);


    char *root_password;
    char *comfirm_root_password;


    int correct_pass = 0;
    while (!correct_pass)
    {
        root_password = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char));
        comfirm_root_password = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char));

        printf("Enter your root password : ");
        root_password = strdup(getpass(""));
        
        printf("Confirm your root password : ");
        comfirm_root_password = strdup(getpass(""));

        if (strcmp(root_password, comfirm_root_password) == 0){
            correct_pass = 1;
        }else{
            printf("Incorrect comfirm password, please try again!\n");
            free(root_password);
            free(comfirm_root_password);
        }
    }

    // cần tạo thư mục etc trước
    mkdir(etc_absolute_path, 0777);

    /// sau đó mới tạo file hostname ///

    // khởi tạo đường dẫn etc/hostname 
    char * host_name_file_path = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char));
    strcpy(host_name_file_path, etc_absolute_path);
    strcat(host_name_file_path,HOST_NAME_FILE);
    // tạo file hostname và ghi tên của hostname vào file
    FILE* file_ptr = fopen(host_name_file_path, "w");
    fputs(computer_name, file_ptr);
    fclose(file_ptr);

    free(host_name_file_path);
    free(computer_name);

    /// sau đó mới tạo file passwd để quản lý users ///
    char * passwd_file_path = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char));
    strcpy(passwd_file_path, etc_absolute_path);
    strcat(passwd_file_path, PASSWD_FILE);
    // tạo file quản lý users
    file_ptr = fopen(passwd_file_path, "w");
    fclose(file_ptr);

    free(passwd_file_path);
    free(etc_absolute_path);

    // thêm root user vào file
    add_user(ADMIN_USER_NAME, root_password, IS_ADMIN, rootPath);
    free(root_password);
    free(comfirm_root_password);


    // tạo thư mục cho root
    char * root_absolute_path;
    root_absolute_path = malloc(strlen(rootPath)+strlen(ROOT_HOME_DIR)+1); 
    strcpy(root_absolute_path, rootPath);
    strcat(root_absolute_path, ROOT_HOME_DIR);

    mkdir(root_absolute_path, 0777);
    free(root_absolute_path);


    // tạo các thư mục cho hệ điều hành //

    // thư mục bin
    char * bin_absolute_path;
    bin_absolute_path = malloc(strlen(rootPath)+strlen(BIN_DIR)+1); 
    strcpy(bin_absolute_path, rootPath);
    strcat(bin_absolute_path, BIN_DIR);

    mkdir(bin_absolute_path, 0777);
    free(bin_absolute_path);

    // thư mục home 
    char * home_absolute_path;
    home_absolute_path = malloc(strlen(rootPath)+strlen(HOME_DIR)+1); 
    strcpy(home_absolute_path, rootPath);
    strcat(home_absolute_path, HOME_DIR);

    mkdir(home_absolute_path, 0777);
    free(home_absolute_path);

    

    

    // làm các thanh trạng thái cho zui
    display_progess_bar(osVersion);
    
    
    
}

void display_progess_bar(const char* osVersion){
    for (int i = 0; i <= 100; i+=5){
        welcome_message(osVersion);
        printf("INSTALLING OS...\n");
        printf("THIS MIGHT TAKE SEVERAL MINUTES...\n");
        printf("PLEASE WAIT...\n");
        progress_bar(i, 100);
        delay(200);
    }
}

