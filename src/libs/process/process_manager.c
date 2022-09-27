/**
 * Thư viên tạo luồng của Aurora5-OS
 * @author Nguyễn Văn Khánh Duy
 * @date 20/09/2022
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../constants.h"

/**
 * Tạo luồng và thực thi câu lệnh
 * @param binPath: đường dẫn tới file thực thi
 * @param argv: các đối số dòng lệnh
 * @return exit code
 */
int run_thread(const char *binPath, char **argv)
{
    int exit_code = ERROR_EXIT_CODE;

    int child_process_id = fork();
    if (child_process_id)
    {
        // Fork trả giá trị của luồng con tại đây
        
        // đợi luồng thực hiện xong
        
        int waitstatus;
        wait(&waitstatus);
        exit_code =  WEXITSTATUS(waitstatus);
        return exit_code;

        
    }
    else
    {
        // luồng thực hiện tại đây
        // thực thi câu lệnh và trả về exit code
        exit_code =  execv(binPath, argv);
        
    }
}