#include "fifo_header.h"
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>

int main(void) {
    int fd_server, fd_client, i;
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

    fd_server = open(SERVER_FIFO_NAME, O_WRONLY);

    for(i = 0; word[i] != NULL; i++) {
        strcpy(msg.sm_data, word[i]);
        write(fd_server, &msg, sizeof(msg));

        if(fd_client == -1)
            fd_client = open(fifo_name, O_RDONLY);

        nread = read(fd_client, &msg, sizeof(msg));

        if(nread == 0) {
            errno = ENETDOWN;
            exit(EXIT_FAILURE);
        }
        printf("Client %ld: %s --> %s\n", (long)getpid(), word[i], msg.sm_data);
    }
    close(fd_server);
    close(fd_client);
    unlink(fifo_name);
    printf("Client %ld logging out \n", (long)getpid());

    exit(EXIT_SUCCESS);
}
