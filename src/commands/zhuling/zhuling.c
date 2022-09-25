#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#include "../../libs/constants.h"
#include "../../libs/utils/common.h"
#include "../../libs/utils/aucolors.h"
#include "../../libs/user/users_manager.h"

const char *VERSION = "1.0.0";
const char *NAME = "zhuling";
const char *CONFIG_DIR = "/zhuling";
const char *SOURCE_FILE = "/sources.list";

struct url_data
{
    size_t size;
    char *data;
};

size_t write_data(void *ptr, size_t size, size_t nmemb, struct url_data *data)
{
    size_t index = data->size;
    size_t n = (size * nmemb);
    char *tmp;

    data->size += (size * nmemb);

#ifdef DEBUG
    fprintf(stderr, "data at %p size=%ld nmemb=%ld\n", ptr, size, nmemb);
#endif
    tmp = realloc(data->data, data->size + 1); /* +1 for '\0' */

    if (tmp)
    {
        data->data = tmp;
    }
    else
    {
        if (data->data)
        {
            free(data->data);
        }
        fprintf(stderr, "Failed to allocate memory.\n");
        return 0;
    }

    memcpy((data->data + index), ptr, n);
    data->data[data->size] = '\0';

    return size * nmemb;
}

char *handle_url(char *url)
{
    CURL *curl;

    struct url_data data;
    data.size = 0;
    data.data = malloc(4096); /* reasonable size initial buffer */
    if (NULL == data.data)
    {
        fprintf(stderr, "Failed to allocate memory.\n");
        return NULL;
    }

    data.data[0] = '\0';

    CURLcode res;

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }
    return data.data;
}

int add_repo(char *repo_url, char *root_path)
{
    if (repo_url[0] == 0)
    {
        printf("Invalid url.\n");
        return ERROR_EXIT_STATUS;
    }

    if (strstr(repo_url, "https://") == NULL)
    {
        printf("Invalid url.\n");
        return ERROR_EXIT_STATUS;
    }

    char *config_path;
    config_path = malloc(strlen(root_path) + strlen(ETC_DIR) + strlen(CONFIG_DIR) + strlen(SOURCE_FILE) + 1);
    strcpy(config_path, root_path);
    strcat(config_path, ETC_DIR);
    strcat(config_path, CONFIG_DIR);

    if (is_dir_exists(config_path) == 0)
    {
        mkdir(config_path, 0777);
    }

    strcat(config_path, SOURCE_FILE);

    FILE *file = fopen(config_path, "a");

    if (file == NULL)
    {
        printf("Something went wrong, try again!\n");
    }

    strcat(repo_url, "\n");

    fputs(repo_url, file);

    fclose(file);

    return SUCCESS_EXIT_CODE;
}

int list_scripts(char *root_path)
{
    char *bin_path;
    bin_path = malloc(strlen(root_path) + strlen(BIN_DIR) + 1);
    strcpy(bin_path, root_path);
    strcat(bin_path, BIN_DIR);
    DIR *direntory;
    struct dirent *entry;
    direntory = opendir(bin_path);
    if (direntory == NULL)
    {
        printf("Something went wrong, try again.\n");
        return ERROR_EXIT_CODE;
    }

    while ((entry = readdir(direntory)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0)
        {
            if (strcmp(entry->d_name, "..") != 0)
            {
                printf("%s\n", entry->d_name);
            }
        }
    }

    printf("\n");

    closedir(direntory);
}

size_t write_data2(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int download_file(char *url, char *filename, char *root_path)
{
    CURL *curl;
    FILE *fp;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();

    char *final_path = malloc(strlen(root_path) + strlen(BIN_DIR) + strlen(filename) + 1);
    strcpy(final_path, root_path);
    strcat(final_path, BIN_DIR);
    strcat(final_path, "/");
    strcat(final_path, filename);

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data2);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        fp = fopen(final_path, "wb");
        if (fp)
        {

            /* write the page body to this file handle */
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

            /* get it! */
            curl_easy_perform(curl);

            /* close the header file */
            fclose(fp);
            if (is_file_exists(final_path) == 0)
            {
                printf("Error when download a file from repo.\n");
                return ERROR_EXIT_CODE;
            }
            chmod(final_path, 0777);
            return SUCCESS_EXIT_CODE;
        }

        curl_easy_cleanup(curl);

        curl_global_cleanup();

        return SUCCESS_EXIT_CODE;
    }
    return ERROR_EXIT_CODE;
}

int install_script(char *script_name, char *root_path)
{
    char *config_path;
    config_path = malloc(strlen(root_path) + strlen(ETC_DIR) + strlen(CONFIG_DIR) + strlen(SOURCE_FILE) + 1);
    strcpy(config_path, root_path);
    strcat(config_path, ETC_DIR);
    strcat(config_path, CONFIG_DIR);
    strcat(config_path, SOURCE_FILE);

    if (is_file_exists(config_path) == 0)
    {
        printf("Something went wrong, try again.\n");
        return NOT_FOUND_EXIT_CODE;
    }

    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(config_path, "r");
    if (fp == NULL)
    {
        printf("Something went wrong, try again.\n");
        return NOT_FOUND_EXIT_CODE;
    }

    printf("Reading repo lists...\n");
    while ((read = getline(&line, &len, fp)) != -1)
    {
        printf("[Repo] %s", line);
        if (line[strlen(line) - 1] == 10)
        {
            line[strlen(line) - 1] = '\0';
        }

        // lấy danh sách script từ repo về
        char *repo_list = handle_url(line);
        char *temp_repo_list = malloc(strlen(repo_list) + 1);
        strcpy(temp_repo_list, repo_list);

        if (temp_repo_list)
        {
            char *header = strtok(temp_repo_list, "\n");
            if (header[strlen(header) - 1] == 10)
            {
                header[strlen(header) - 1] = '\0';
            }
            if (strcmp(header, "[zhuling]") != 0)
            {
                printf("Invalid repository list.\n\n");
            }
            else
            {
                char *start = &repo_list[9];
                char *end = &repo_list[strlen(repo_list) - 1];

                char *substr = (char *)calloc(1, end - start + 1);
                memcpy(substr, start, end - start);

                char *curLine = substr;
                while (curLine)
                {
                    char *nextLine = strchr(curLine, '\n');

                    if (nextLine)
                    {
                        *nextLine = '\0';
                    }
                    if (curLine[0] != '\0')
                    {
                        char *str_tmp = malloc(strlen(curLine) + 1);
                        strcpy(str_tmp, curLine);
                        char *token = strtok(str_tmp, ";");
                        if (strcmp(token, script_name) == 0)
                        {
                            char *filename = malloc(strlen(token) + 1);
                            strcpy(filename, token);

                            // tải script
                            token = strtok(NULL, ";");
                            if (token[strlen(token) - 1] == 10)
                            {
                                token[strlen(token) - 1] = '\0';
                            }
                            printf("Downloading...\n");
                            return download_file(token, filename, root_path);
                        }
                    }
                    if (nextLine)
                    {
                        *nextLine = '\n';
                    }
                    curLine = nextLine ? (nextLine + 1) : NULL;
                }
                printf("Unable to locate script %s\n", script_name);
                return NOT_FOUND_EXIT_CODE;
            }
        }
        else
        {
            printf("Error when mirror to '%s'\n", line);
            return ERROR_EXIT_CODE;
        }
    }

    fclose(fp);
    if (line)
        free(line);
    printf("Something went wrong, try again.\n");
    return ERROR_EXIT_CODE;
}

int remove_script(char *script_name, char *root_path)
{
    char *final_path = malloc(strlen(root_path) + strlen(BIN_DIR) + strlen(script_name) + 1);
    strcpy(final_path, root_path);
    strcat(final_path, BIN_DIR);
    strcat(final_path, "/");
    strcat(final_path, script_name);

    if (is_file_exists(final_path) == 0){
        printf("Unable to locate script %s\n", script_name);
        return NOT_FOUND_EXIT_CODE;
    }

    remove(final_path);
    printf("Sucessful!\n");
    return SUCCESS_EXIT_CODE;

}

int main(int argc, char *argv[])
{
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

    for (int i = 0; i < argc; i++)
    {
        if (argv[i][strlen(argv[i]) - 1] == 10)
        {
            argv[i][strlen(argv[i]) - 1] = '\0';
        }
    }

    if (argc <= 3)
    {
        if (strcmp(argv[1], "list") == 0)
        {
            return list_scripts(argv[argc - 1]);
        }
        printf("%s v%s\n", NAME, VERSION);
        printf("Usage: %s command [options]\n\n", NAME);
        printf("Commands: \n");
        printf(" add-repo : Add entries to %s sources.list\n", NAME);
        printf(" install  : Install/upgrade a script\n");
        printf(" remove   : Remove a script\n");
        printf(" list     : List all scripts\n\n");
        return ERROR_EXIT_CODE;
    }

    if (argc > 4)
    {
        printf("%s: Error: to many arguments, try again!\n", NAME);
        return ERROR_EXIT_CODE;
    }
    // for (int i =0;i<argc;i++){
    //     printf("%s\n",argv[i]);
    // }
    if (strcmp(argv[1], "add-repo") == 0)
    {
        return add_repo(argv[2], argv[argc - 1]);
    }
    if (strcmp(argv[1], "install") == 0)
    {
        return install_script(argv[2], argv[argc - 1]);
    }
    if (strcmp(argv[1], "remove") == 0)
    {
        return remove_script(argv[2], argv[argc - 1]);
    }
    printf("Unknown: %s\n",argv[2]);
    return ERROR_EXIT_CODE;
}