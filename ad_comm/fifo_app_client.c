#include "fifo_header.h"
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>

int main(void) {
    int fd_server, fd_client = -1, fd_client_w = -1, i;
    ssize_t nread;
    struct simple_message msg;
    char fifo_name[100];
    mode_t perm = 0666;
    char *word[] = {
        "applesauce",
        "tiger",
        "mountain",
        NULL
    };

    printf("Client %ld started\n", (long)getpid());
    msg.sm_clientpid = getpid();
    make_fifo_name(msg.sm_clientpid, fifo_name, sizeof(fifo_name));

    if (mkfifo(SERVER_FIFO_NAME, perm) == -1 && errno != EEXIST)
        printf("ERROR\n");

    printf("Client name: %s connecting to %s\n", fifo_name, SERVER_FIFO_NAME);
    fd_server = open(SERVER_FIFO_NAME, O_WRONLY);
    printf("Connected\n");

    for(i = 0; word[i] != NULL; i++) {
        printf("Client sending %s\n", word[i]);
        strcpy(msg.sm_data, word[i]);
        write(fd_server, &msg, sizeof(msg));
        printf("Write success\n");

        if(fd_client == -1) {
            printf("Open fd_client at %s\n", fifo_name);
            fd_client = open(fifo_name, O_RDONLY);
        }

        if(fd_client_w == -1) {
            printf("Open fd_client_w at %s\n", fifo_name);
            fd_client_w = open(fifo_name, O_WRONLY);
        }

        printf("Reading from %d %d\n", fd_client, fd_client_w);
        nread = read(fd_client, &msg, sizeof(msg));

        if(nread == 0) {
            printf("Read error.\n");
            errno = ENETDOWN;
            exit(EXIT_FAILURE);
        }
        printf("Client %ld: %s --> %s\n", (long)getpid(), word[i], msg.sm_data);
    }
    close(fd_server);
    close(fd_client);
    close(fd_client_w);
    unlink(fifo_name);
    printf("Client %ld logging out \n", (long)getpid());

    exit(EXIT_SUCCESS);
}
