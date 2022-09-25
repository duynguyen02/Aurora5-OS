/**
 * @author Nguyễn Phùng Hưng
 * Câu lệnh xóa nội dung ở màn hình
 */
#include "stdio.h"

#include "../../libs/constants.h"

int main(){
    // In mã xóa màn hình
    printf("\e[1;1H\e[2J");
    return SUCCESS_EXIT_CODE;
}