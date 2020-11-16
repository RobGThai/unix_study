#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>

void fsort(void) {
    int pfdout[2], pfdin[2], fd;
    ssize_t nread;
    pid_t pid;
    char buf[512];

    pipe(pfdout);
    pipe(pfdin);
    pid = fork();

    if(pid == 0) {
        dup2(pfdout[0], STDIN_FILENO);
        close(pfdout[0]);
        close(pfdout[1]);
        dup2(pfdin[1], STDOUT_FILENO);
        close(pfdin[0]);
        close(pfdin[1]);
        execlp("sort", "sort", (char *)NULL);
    }

    close(pfdout[0]);
    close(pfdin[1]);

    fd = open("datafile", O_RDONLY);
    while(true) {
        nread = read(fd, buf, sizeof(buf));

        if(nread == 0)
            break;

        // Writing to Child process
        write(pfdout[1], buf, nread);
    }

    close(fd);
    close(pfdout[1]);

    while(true) {
        // Reading from Child process
        nread = read(pfdin[0], buf, sizeof(buf));

        if(nread == 0)
            break;

        write(STDOUT_FILENO, buf, nread);
    }

    close(pfdin[0]);
    waitpid(pid, NULL, 0);

    return;

}

int main() {
    fsort();
}
