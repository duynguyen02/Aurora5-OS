/**
 * Chứa các ràng buộc cho Aurora5-OS
 */

#ifndef CONSTANTS_H_
    #define CONSTANTS_H_

    // các ràng buộc bộ nhớ //
    #define MAX_CWD_SIZE 255
    #define MAX_BUFFER_SIZE 255

    // định nghĩa quản trị viên //
    #define ADMIN_USER_NAME "root"
    #define IS_NOT_ADMIN 0
    #define IS_ADMIN 1

    // định nghĩa các EXIT_STATUS //
    #define SUCESS_EXIT_STATUS 0
    #define ERROR_EXIT_STATUS 2
    #define EXIT_STATUS 1


    #define SUCCESS_EXIT_CODE 0
    #define EXIT_EXIT_CODE 128
    #define ERROR_EXIT_CODE 1
    #define MISUSE_EXIT_CODE 2
    #define CAN_NOT_EXEC_EXIT_CODE 126
    #define NOT_FOUND_EXIT_CODE 127







    // định nghĩa các thành phần thư mục etc //
    #define ETC_DIR "/etc"
    // các file thư mục con
    #define HOST_NAME_FILE "/hostname"
    #define PASSWD_FILE "/passwd"
    #define USER_SHELL_FILE "/usershell.bin"


    // định nghĩa các thành phần thư mục bin //
    #define BIN_DIR "/bin"

    // định nghĩa các thành phần thư mục home //
    #define HOME_DIR "/home"

    // định nghĩa các thành phần thư mục root //
    #define ROOT_HOME_DIR "/root"

#endif