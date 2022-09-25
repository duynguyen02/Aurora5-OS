/**
 * @author Nguyễn Văn Khánh Duy
 * @date 20/09/2022
 * Câu lệnh đăng xuất người dùng khỏi shell
 * @details the deprecated script.
 */

#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define ETC_DIR "/etc"
#define USER_SHELL_FILE "/usershell.bin"

#define MAX_OF_USER 100
#define MAX_BUFFER_SIZE 100

#define AUCOLORS_H_
#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"

typedef struct
{
   char current_user[MAX_BUFFER_SIZE];
   char host_name[MAX_BUFFER_SIZE];
   char root_dir[MAX_BUFFER_SIZE];
   char current_dir[MAX_BUFFER_SIZE];
} UserInfo;

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

int delete_user_shell(const char *rootPath)
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
      return 2;
   }

   unsigned int n_stud = 0;

   UserInfo users[MAX_OF_USER];

   while (fread(&users[n_stud], sizeof(UserInfo), 1, file) == 1)
   {
      n_stud++;
   }

   fclose(file);

   // nếu không còn người dùng nào thì thoát shell
   if (n_stud == 0)
   {
      return 1;
   }

   // còn thì xóa người dùng theo cơ chế stack

   remove(user_shell_path);
   file = fopen(user_shell_path, "w");

   if (file == NULL)
   {
      return 2;
   }


   if (n_stud >= 2){
      UserInfo renew_users[n_stud-1];
      for (int i = 0;i < n_stud -1 ; i++){
         renew_users[i] = users[i];
      }
      fwrite(renew_users,sizeof(UserInfo), n_stud-1, file);

      fclose(file);
      return 0;
   }

   fclose(file);

   

   return 1;
}

int exit_user(const char *rootPath)
{
   return delete_user_shell(rootPath);
}

int main(int argc, char **argv)
{
   if (argc == 2)
   {
      return exit_user(argv[1]);
   }

   return (2);
}