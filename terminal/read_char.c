#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h> //termios, ICANON, ECHO

#define ESC 27

int getch(void) {
    char c;
    switch(read(STDIN_FILENO, &c, 1)) {
        default:
            errno = 0;
        case -1:
            return -1;
        case 1:
            break;
    }
    return c;
}

int main() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO); // Turn off Canonical input and Echo.
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int i;
    while((i = getch()) != ESC) {
        printf("Type: %d\n", i);
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return 0;
}
