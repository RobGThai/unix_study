#include "fifo_header.h"
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

int main(void) {
    int fd_server, fd_client, i;
    ssize_t nread;
    struct simple_message msg;
    char fifo_name[100];
    mode_t perm = 0666;

    printf("Server started\n");
    
    if (mkfifo(SERVER_FIFO_NAME, perm) == -1 && errno != EEXIST)
        printf("ERROR\n");

    fd_server = open(SERVER_FIFO_NAME, O_RDONLY);

    while (true) {
        nread = read(fd_server, &msg, sizeof(msg));
        if(nread == 0) {
            errno = ENETDOWN;
            exit(EXIT_FAILURE);
        }

        for(i = 0; msg.sm_data[i] != '\0'; i++) {
            msg.sm_data[i] = toupper(msg.sm_data[i]);
        }
        make_fifo_name(msg.sm_clientpid, fifo_name, sizeof(fifo_name));
        fd_client = open(fifo_name, O_WRONLY);
        write(fd_client, &msg, sizeof(msg));
        close(fd_client);
    }

    close(fd_server);

    exit(EXIT_SUCCESS);
}
