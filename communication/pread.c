#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int fd;
    ssize_t nread;
    char s[100];

    fd = atoi(argv[1]); // Convert FD sent from pipe.c to FD.
    printf("reading file descriptor %d\n", fd);
    nread = read(fd, s, sizeof(s));
    if(nread == 0)
        printf("EOF\n");
    else
        printf("read %ld bytes: %s\n", (long)nread, s);

    exit(EXIT_SUCCESS);
}

