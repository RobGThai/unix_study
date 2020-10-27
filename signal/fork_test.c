#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int main() {
    pid_t pid;
    pid = fork();

    printf("PID: %d\n", pid);

    return 0;
}
