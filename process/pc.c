#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include "display.h"

static bool wait_and_display(void) {
//     pid_t wpid;
//     int status;
// 
//     wpid = waitpid(-1, &status, 0);
//     display_status(wpid, status);
// 
    siginfo_t info;

    waitid(P_ALL, 0, &info, WEXITED);
    display_status_s(&info);

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
