# define SERVER_FIFO_NAME "fifo_server"

#include <sys/types.h>
#include <stdio.h>
#include <stdbool.h>

struct simple_message {
    pid_t sm_clientpid;
    char sm_data[200];
};

bool make_fifo_name(pid_t pid, char *name, size_t name_max) {
    snprintf(name, name_max, "fifo%ld", (long)pid);
    return true;
};

