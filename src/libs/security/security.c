/**
 * Thư viện mã hóa của Aurora5-OS
 * @author Nguyễn Văn Khánh Duy
 * @date 15/09/2022
 */

#include <crypt.h>
#include <stdio.h>
#include <string.h>
#include "security.h"

char * encrypt_sha512(char * content);

const char * SECRET_KEY = "YEU_EM_LA_SAI_THI_ANH_KHONG_CAN_DUNG";

/**
 * Mã hóa chuỗi ký tự theo chuẩn SHA-512
 * @param content: chuỗi ký tự cần mã hóa
 * @return chuỗi ký tự đã được mã hóa
 */
char * encrypt_sha512(char * content){
    struct crypt_data cd;
    memset(&cd, 0, sizeof cd);
    return crypt_r(content, SECRET_KEY, &cd);
}