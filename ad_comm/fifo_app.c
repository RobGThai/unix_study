#include "fifo_header.h"
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

int main(void) {
    int fd_server, fd_server_w, fd_client, i;
    ssize_t nread;
    struct simple_message msg;
    char fifo_name[100];
    mode_t perm = 0666;

    printf("Server started\n");
    
    if (mkfifo(SERVER_FIFO_NAME, perm) == -1 && errno != EEXIST)
        printf("ERROR\n");

    printf("Reading from %s\n", SERVER_FIFO_NAME);
    fd_server = open(SERVER_FIFO_NAME, O_RDONLY);
    fd_server_w = open(SERVER_FIFO_NAME, O_WRONLY);

    printf("Waiting...\n");
    while (true) {
        nread = read(fd_server, &msg, sizeof(msg));
        if(nread == 0) {
            printf("Cannot read data\n");
            errno = ENETDOWN;
            exit(EXIT_FAILURE);
        }

        printf("Recieve %s\n", msg.sm_data);

        for(i = 0; msg.sm_data[i] != '\0'; i++) {
            msg.sm_data[i] = toupper(msg.sm_data[i]);
        }
        make_fifo_name(msg.sm_clientpid, fifo_name, sizeof(fifo_name));
        printf("Open client to write %s back at %s.\n", msg.sm_data, fifo_name);
        fd_client = open(fifo_name, O_WRONLY);
        printf("Sending to %d\n", fd_client);
        write(fd_client, &msg, sizeof(msg));
        printf("Closing %d\n", fd_client);
        close(fd_client);
    }

    close(fd_server);
    close(fd_server_w);

    exit(EXIT_SUCCESS);
}
