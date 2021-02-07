#define SOCKETNAME "MySocket"

#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    struct sockaddr_un sa;

    (void) unlink(SOCKETNAME);
    strcpy(sa.sun_path, SOCKETNAME);
    sa.sun_family = AF_UNIX;
    if(fork() == 0) {
        int fd_skt;
        char buf[100];

        fd_skt = socket(AF_UNIX, SOCK_STREAM, 0);
        while(connect(fd_skt, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
            if(errno == ENOENT) {
                sleep(1);
                continue;
            }else {
                // Error
            }
        }
        write(fd_skt, "Hello!", 7);
        read(fd_skt, buf, sizeof(buf));
        printf("Client got \"%s\"\n", buf);
        close(fd_skt);
        exit(EXIT_SUCCESS);
    }else {
        int fd_skt, fd_client;
        char buf[100];

        fd_skt = socket(AF_UNIX, SOCK_STREAM, 0);
        bind(fd_skt, (struct sockaddr *)&sa, sizeof(sa));
        listen(fd_skt, SOMAXCONN);
        fd_client = accept(fd_skt, NULL, 0);
        read(fd_client, buf, sizeof(buf));
        printf("Server got \"%s\" \n", buf);
        write(fd_client, "Goodbye!", 9);
        close(fd_skt);
        close(fd_client);
        exit(EXIT_SUCCESS);
    }
}
