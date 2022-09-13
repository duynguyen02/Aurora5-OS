#include "stdlib.h"
#include "string.h"

#include "libs/setup/shell.h"
#include "libs/setup/FirstSetup.h"
#include "libs/utils/Common.h"


#include "stdio.h"


const char* OS_NAME = "Aurora5-OS";

const char* OS_VERSION = "1.0.0";

const char* ETC_DIR = "/etc";

int main(){
    // hiển thị tin nhắn chào mừng
    welcome_message(OS_VERSION);

    // lấy thư mục hiện hành (giả định là thư mục gốc)
    char * ROOT_DIR = get_current_dir();

    // thiết lập thư mục hiện hành cho hệ điều hành là thư mục gốc
    char * CURRENT_DIR = ROOT_DIR;

    // nếu thư mục etc chưa tồn tại, giả định hệ điều hành chưa được cài đặt
    // bắt đầu thiết lập cài đặt hệ điều hành
    char * etc_absolute_path;
    etc_absolute_path = malloc(strlen(ROOT_DIR)+strlen(ETC_DIR)+1); 
    strcpy(etc_absolute_path, ROOT_DIR);
    strcat(etc_absolute_path, ETC_DIR);


    if(is_dir_exists(etc_absolute_path) == 0){
        first_time_setup(
            OS_NAME,
            OS_VERSION
        );
    }




    return 0;
}