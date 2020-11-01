#include <stdio.h>
#include <sys/wait.h>

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
    }
}

void display_status_s(siginfo_t *infop) {
    printf("Process %ld terminated:\n", (long) infop->si_pid);
    printf("\tcode = %d\n", infop->si_code);
    if(infop->si_code == CLD_EXITED)
        printf("\texit value = %d\n", infop->si_status);
    printf("\terror = %d\n", infop->si_errno);
}
