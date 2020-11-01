#include "display.h"

void display_status(pid_t pid, int status) {
    if(pid != 0)
        printf("Process %ld: ", (long)pid);
    if(WIFEXITED(status))
        printf("Exit value %d %d\n", status, WEXITSTATUS(status));
    else {
        printf("Killed by signal: %d\n", WTERMSIG(status));

//         if(WCOREDUMP(status))
//             printf(" - core dumped");
//         if(WIFSTOPPED(status))
//             printf(" (stopped)");
// #if defined(_XOPEN_UNIX) && !defined(LINUX)
//         else if(WIFCONTINUED(status))
//             printf(" (continued)");
// #endif
        printf("\n");
    }
}

