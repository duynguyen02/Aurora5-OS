#include <crypt.h>
#include <stdio.h>
#include <string.h>
#include "security.h"

const char * SECRET_KEY = "NGUOI_THU_3_LA_CON_CHO_DE";

char * encrypt_sha512(char * content){
    struct crypt_data cd;
    memset(&cd, 0, sizeof cd);
    return crypt_r(content, SECRET_KEY, &cd);
}