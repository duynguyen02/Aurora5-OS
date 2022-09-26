/**
 * @author Nguyễn Phạm Xuân Hiền
 * Đăng xuất người dùng khỏi shell
 */

#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "../../libs/constants.h"
#include "../../libs/user/users_manager.h"

/**
 * Xóa người dùng khỏi shell
 */
int delete_user_shell(const char *rootPath)
{
   // tạo đường dẫn tới tập usershell.bin
   char *user_shell_path = calloc(strlen(rootPath) + strlen(ETC_DIR) + strlen(USER_SHELL_FILE) + 1, sizeof(char));
   strcpy(user_shell_path, rootPath);
   strcat(user_shell_path, ETC_DIR);
   strcat(user_shell_path, USER_SHELL_FILE);

   // đọc danh sách người dùng đang đăng nhập shell
   FILE *file;
   file = fopen(user_shell_path, "r");

   if (file == NULL)
   {
      return NOT_FOUND_EXIT_CODE;
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
      return ERROR_EXIT_CODE;
   }

   // còn thì xóa người dùng theo cơ chế stack
   remove(user_shell_path);
   file = fopen(user_shell_path, "w");

   if (file == NULL)
   {
      return NOT_FOUND_EXIT_CODE;
   }

   if (n_stud >= 2)
   {
      UserInfo renew_users[n_stud - 1];
      for (int i = 0; i < n_stud - 1; i++)
      {
         renew_users[i] = users[i];
      }
      fwrite(renew_users, sizeof(UserInfo), n_stud - 1, file);

      fclose(file);
      return SUCCESS_EXIT_CODE;
   }

   fclose(file);

   return ERROR_EXIT_CODE;
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

   return ERROR_EXIT_CODE;
}