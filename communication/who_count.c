#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

long check_buffer_size(int pfd) {
    long v = fpathconf(pfd, _PC_PIPE_BUF);

    if(v == -1) {
        printf("No PIPE_BUF limit.\n");    
    }else {
        printf("PIPE_BUF = %ld\n", v);
    }

    return v;
}

void pipetest(int pread, int pwrite) {
    ssize_t nread;
    char s[100];

    write(pwrite, "hello", 6);
    nread = read(pread, s, sizeof(s));
    if(nread == 0)
        printf("EOF\n");
    else
        printf("read %ld bytes: %s\n", (long)nread, s);
}

void pipewrite(int readfd, int writefd) {
    char fdstr[10];

    switch(fork()) {
        case -1:
            printf("Fork error.\n");
        case 0: /* Child */
            close(writefd);
            // Convert pread into string to pass to pread.
            snprintf(fdstr, sizeof(fdstr), "%d", readfd);
            execlp("./pread.o", "piperead", fdstr, (char *)NULL);
            printf("Failed if reached.\n");
        default:
            close(readfd);
            write(writefd, "hello", 6);
    }
}

void test() {
    int pfd[2];

    int result = pipe(pfd);
    printf("Pipe creation result: %d\n", result);
    check_buffer_size(pfd[0]);
//     Commented because the limit is from system level.
//     check_buffer_size(pfd[1]);
    printf("--------------------\n");
    pipetest(pfd[0], pfd[1]);

}

int main(int argc, char *argv[]) {
    int pfd[2];
    pid_t pid1, pid2;

    pipe(pfd);
    switch(pid1 = fork()){
        case -1:
            printf("Fork error.\n");
        case 0: /* Child */
            dup2(pfd[1], STDOUT_FILENO);
            close(pfd[1]);
            close(pfd[0]);
            // Convert pread into string to pass to pread.
            // execlp("who", "who", NULL);
            execlp("ls", "ls", NULL);
            printf("Failed if reached.\n");
    }

    switch(pid2 = fork()){
        case -1:
            printf("Fork error.\n");
        case 0: /* Child */
            dup2(pfd[0], STDIN_FILENO);
            close(pfd[1]);
            close(pfd[0]);
            // Convert pread into string to pass to pread.
            execlp("wc", "wc", "-l", NULL);
            printf("Failed if reached.\n");
    }

    close(pfd[0]);
    close(pfd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    return EXIT_SUCCESS;
}

