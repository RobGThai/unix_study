#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

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

int main(void) {
    int pfd[2];

    int result = pipe(pfd);
    printf("Pipe creation result: %d\n", result);
    check_buffer_size(pfd[0]);
//     Commented because the limit is from system level.
//     check_buffer_size(pfd[1]);
    printf("--------------------\n");
    pipetest(pfd[0], pfd[1]);
    return EXIT_SUCCESS;
}
