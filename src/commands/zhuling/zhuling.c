#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../../libs/constants.h"
#include "../../libs/utils/common.h"
#include "../../libs/utils/aucolors.h"
#include "../../libs/user/users_manager.h"


const char * VERSION = "1.0.0";
const char * NAME = "zhuling";
const char * CONFIG_DIR = "/zhuling";
const char * SOURCE_FILE = "/sources.list";

struct url_data {
    size_t size;
    char* data;
};

size_t write_data(void *ptr, size_t size, size_t nmemb, struct url_data *data) {
    size_t index = data->size;
    size_t n = (size * nmemb);
    char* tmp;

    data->size += (size * nmemb);

#ifdef DEBUG
    fprintf(stderr, "data at %p size=%ld nmemb=%ld\n", ptr, size, nmemb);
#endif
    tmp = realloc(data->data, data->size + 1); /* +1 for '\0' */

    if(tmp) {
        data->data = tmp;
    } else {
        if(data->data) {
            free(data->data);
        }
        fprintf(stderr, "Failed to allocate memory.\n");
        return 0;
    }

    memcpy((data->data + index), ptr, n);
    data->data[data->size] = '\0';

    return size * nmemb;
}

char *handle_url(char* url) {
    CURL *curl;

    struct url_data data;
    data.size = 0;
    data.data = malloc(4096); /* reasonable size initial buffer */
    if(NULL == data.data) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return NULL;
    }

    data.data[0] = '\0';

    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n",  
                        curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);

    }
    return data.data;
}

int add_repo(char * repo_url, char * root_path){
    char *config_path;
    config_path = malloc(strlen(root_path) + strlen(ETC_DIR) + strlen(CONFIG_DIR) + strlen(SOURCE_FILE) + 1);
    strcpy(config_path, root_path);
    strcat(config_path, ETC_DIR);
    strcat(config_path, PASSWD_FILE);
    
    if (is_dir_exists(config_path) == 0){
        mkdir(config_path, 0777);
    }

    strcat(config_path, SOURCE_FILE);

    FILE * file = fopen(config_path, "a");

    fputs(repo_url, file);

    fclose(file);

    return SUCESS_EXIT_STATUS;
}


int main(int argc, char* argv[]) {
    // char* data;

    // if(argc < 2) {
    //     fprintf(stderr, "Must provide URL to fetch.\n");
    //     return 1;
    // }
    // data = handle_url(argv[1]);

    // if(data) {
    //     printf("%s\n", data);
    //     free(data);
    // }


    for (int i = 0; i < argc; i++){
        if (argv[i][strlen(argv[i]) - 1] == 10){
            argv[i][strlen(argv[i]) - 1] = '\0';
        }
    }

    if (argc <= 2){
        printf("%s v%s\n", NAME ,VERSION);
        printf("Usage: %s command [options]\n\n", NAME);
        printf("Commands: \n");
        printf(" add-repo : Add entries to %s sources.list\n", NAME);
        printf(" install  : Install/upgrade a script\n");
        printf(" remove   : Remove a script\n");
        printf(" list     : List all scripts\n\n");
        return ERROR_EXIT_STATUS;
    }

    if (argc > 4){
        printf("%s: Error: to many arguments, try again!\n", NAME);
        return ERROR_EXIT_STATUS;
    }

    if (strcmp(argv[1], "add-repo") == 0){
        return add_repo(argv[2], argv[argc - 1]);
    }







    return 0;
}