#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/wait.h>

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

static bool wait_and_display(void) {
    pid_t wpid;
    int status;

    wpid = waitpid(-1, &status, 0);
    display_status(wpid, status);

    return true;
}


int main(void) {
    pid_t pid;

    /* Case 1: Explicit call to _exit */
    if(fork() == 0) /* child */
        _exit(123);
    wait_and_display();

    /* Case 2: Termination by kernel */
    if(fork() == 0) {
        int a, b = 0;
        a = 1 / b;
        _exit(EXIT_SUCCESS);
    }

    wait_and_display();

    if((pid = fork()) == 0) {
        sleep(1000);
        _exit(EXIT_SUCCESS);
    }

    kill(pid, SIGHUP);
    wait_and_display();

    exit(EXIT_SUCCESS);
}
