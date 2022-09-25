#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../constants.h"

typedef struct
{
    char *path;
    char **argv;
} compute_prime_struct;

int run_thread(const char *binPath, char **argv)
{
    int exit_code = ERROR_EXIT_CODE;

    int child_process_id = fork();
    if (child_process_id)
    {
        // Fork returns a valid pid in the parent process.  Parent executes this.

        // wait for the child process to complete
        // waitpid(child_process_id) // omitted extra args for brevity
        int waitstatus;
        wait(&waitstatus);
        exit_code =  WEXITSTATUS(waitstatus);
        return exit_code;

        // child process finished!
    }
    else
    {
        // Fork returns 0 in the child process.  Child executes this.

        // new argv array for the child process

        // now start executing some other program
        exit_code =  execv(binPath, argv);
        
    }
}