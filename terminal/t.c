#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    char *tty = ttyname(STDIN_FILENO);
    printf("TTY IN(%d): %s\n", STDIN_FILENO, tty);

    tty = ttyname(STDOUT_FILENO);
    printf("TTY OUT(%d): %s\n", STDOUT_FILENO, tty);

    tty = ttyname(STDERR_FILENO);
    printf("TTY ERR(%d): %s\n", STDERR_FILENO, tty);

    return 0;
}
