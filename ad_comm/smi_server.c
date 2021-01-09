#define SERVER_NAME "smsg_server"
#define DATA_SIZE 200
#define MAX_CLIENTS 20
#define PERM_FILE 0666
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "smi.h"

typedef struct {
    SMIENTITY sq_entity;
    int sq_fd_server;
    int sq_fd_server_w;
    char sq_name[SERVER_NAME_MAX];
    struct {
        int cl_fd;
        pid_t cl_pid;
    } sq_clients[MAX_CLIENTS];
    struct client_id sq_client;
    size_t sq_msgsize;
    struct smi_msg * sq_msg;
} SMIQ_FIFO;

// Client management functions
static void clients_bgn(SMIQ_FIFO *p) {
    int i;

    for (i = 0; i < MAX_CLIENTS; i++)
        p->sq_clients[i].cl_fd = -1;
}

static void clients_close_all(SMIQ_FIFO *p) {
    int i;
    for (i = 0; i < MAX_CLIENTS; i++) {
        if(p->sq_clients[i].cl_fd != -1) {
            (void) close(p->sq_clients[i].cl_fd);
            p->sq_clients[i].cl_fd = -1;
        }
    }
}

static void clients_end(SMIQ_FIFO *p) {
    clients_close_all(p);
}

static int clients_find(SMIQ_FIFO *p, pid_t pid) {
    int i, avail = -1;

    for(i = 0; i < MAX_CLIENTS; i++) {
        if(p->sq_clients[i].cl_pid == pid)
            return i;

        if(p->sq_clients[i].cl_fd == -1 && avail == -1)
            avail = i;
    }

    if(avail != -1)
        p->sq_clients[avail].cl_pid = pid;

    return avail;
}

/*
 *  Function: setblock
 *  ------------------
 *  Control blocking(O_NONBLOCK) flag on the given file descriptor.
 */
bool setblock(int fd, bool block) {
    int flags;
    flags = fcntl(fd, F_GETFL);
    if(block)
        flags &= ~O_NONBLOCK;
    else
        flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flags);

    return true;
}

// FIFO related functions

static void make_fifo_name_server(const SMIQ_FIFO *p, char *fifoname, size_t fifoname_max) {
    snprintf(fifoname, fifoname_max, "/tmp/smififo-%s", p->sq_name);
}

static void make_fifo_name_client(pid_t pid, char *fifoname, size_t fifoname_max) {
    snprintf(fifoname, fifoname_max, "/tmp/smififo%ld", (long)pid);
}

SMIQ *smi_open_fifo(const char *name, SMIENTITY entity, size_t msgsize) {
    SMIQ_FIFO *p = NULL;
    char fifoname[SERVER_NAME_MAX + 50];

    p = calloc(1, sizeof(SMIQ_FIFO));
    p->sq_msgsize = msgsize + offsetof(struct smi_msg, smi_data);
    p->sq_msg = calloc(1, p->sq_msgsize);
    p->sq_entity = entity;

    if(strlen(name) >= SERVER_NAME_MAX) {
        errno = ENAMETOOLONG;
        printf("ERROR\n");
    }
    
    strcpy(p->sq_name, name);
    make_fifo_name_server(p, fifoname, sizeof(fifoname));

    if(p->sq_entity == SMI_SERVER) {
        clients_bgn(p);

        if(mkfifo(fifoname, PERM_FILE) == -1 && errno != EEXIST)
            printf("ERROR mkfifo\n");
        p->sq_fd_server = open(fifoname, O_RDONLY);
        p->sq_fd_server_w = open(fifoname, O_WRONLY);
    }else{
        p->sq_fd_server = open(fifoname, O_WRONLY);
        make_fifo_name_client(getpid(), fifoname, sizeof(fifoname));
        (void)unlink(fifoname);
        mkfifo(fifoname, PERM_FILE);
        p->sq_clients[0].cl_fd = open(fifoname, O_RDONLY | O_NONBLOCK);
        setblock(p->sq_clients[0].cl_fd, true);
        p->sq_clients[0].cl_fd = open(fifoname, O_WRONLY);
    }

    return (SMIQ *)p;
}

bool smi_close_fifo(SMIQ *sqp) {
    SMIQ_FIFO *p = (SMIQ_FIFO *)sqp;
    clients_end(p);
    (void)close(p->sq_fd_server);
    if(p->sq_entity == SMI_CLIENT) {
        char fifoname[SERVER_NAME_MAX + 50];

        make_fifo_name_client(getpid(), fifoname, sizeof(fifoname));
        (void)unlink(fifoname);
    }else {
        (void)close(p->sq_fd_server_w);
    }
    free(p->sq_msg);
    free(p);

    return true;
}

bool smi_send_getaddr_fifo(SMIQ *sqp, struct client_id *client, void **addr) {
    SMIQ_FIFO *p = (SMIQ_FIFO *) sqp;
    if(p->sq_entity == SMI_SERVER)
        p->sq_client = *client;
    *addr = p->sq_msg;

    return true;
}

bool smi_send_release(SMIQ *sqp) {
    SMIQ_FIFO *p = (SMIQ_FIFO *)sqp;
    ssize_t nwrite;

    if(p->sq_entity == SMI_SERVER) {
        int nclient = clients_find(p, p->sq_client.c_id1);
        if(nclient == -1 || p->sq_clients[nclient].cl_fd == -1) {
            errno = EADDRNOTAVAIL;
            printf("Over capacity, cannot send message.\n");
        }
        nwrite = write(p->sq_clients[nclient].cl_fd, p->sq_msg, p->sq_msgsize);
    }else{
        p->sq_msg->smi_client.c_id1 = (long) getpid();
        nwrite = write(p->sq_fd_server, p->sq_msg, p->sq_msgsize);
    }

    return true;
}

bool smi_receive_getaddr_fifo(SMIQ *sqp, void **addr) {
    SMIQ_FIFO *p = (SMIQ_FIFO *)sqp;
    ssize_t nread;

    if(p->sq_entity == SMI_SERVER) {
        int nclient;
        char fifoname[SERVER_NAME_MAX + 50];

        while(true) {
            nread = read(p->sq_fd_server, p->sq_msg, p->sq_msgsize);

            if(nread == 0) {
                errno = ENETDOWN;
                printf("Read fail.\n");
            }

            if(nread < offsetof(struct smi_msg, smi_data)) {
                errno = E2BIG;
                printf("Error: Unexpected data.\n");
            }

            if((nclient = clients_find(p, (pid_t)p->sq_msg->smi_client.c_id1)) == -1) {
                // Over capacity, cannot send message.
                // Client will be blocked indefinitely because of blocking FD.
                continue; 
            }

            if(p->sq_clients[nclient].cl_fd == -1) {
                make_fifo_name_client((pid_t)p->sq_msg->smi_client.c_id1, fifoname, sizeof(fifoname));
                p->sq_clients[nclient].cl_fd = open(fifoname, O_WRONLY);
            }
            break;
        }
    }else{
        nread = read(p->sq_clients[0].cl_fd, p->sq_msg, p->sq_msgsize);
    }
    *addr = p->sq_msg;

    return true;
}

bool smi_receive_release_fifo(SMIQ *sqp) {
    return true;
}

// Interface implementation wrapper functions

SMIQ *smi_open(const char *name, SMIENTITY entity, size_t msgsize) {
    return smi_open_fifo(name, entity, msgsize);
}

bool smi_send_getaddr(SMIQ *sqp, struct client_id *client, void **addr) {
    return smi_send_getaddr_fifo(sqp, client, addr);
}

bool smi_receive_getaddr(SMIQ *sqp, void **addr) {
    return smi_receive_getaddr_fifo(sqp, addr);
}

bool smi_receive_release(SMIQ *sqp) {
    return smi_receive_release_fifo(sqp);
}

int main(void) {
    SMIQ *sqp;
    struct smi_msg *msg_in, *msg_out;
    int i;

    printf("server started MAX(%d)\n", SERVER_NAME_MAX);
    sqp = smi_open(SERVER_NAME, SMI_SERVER, DATA_SIZE);
    while(true) {
        smi_receive_getaddr(sqp, (void **)&msg_in);
        smi_send_getaddr(sqp, &msg_in->smi_client, (void **)&msg_out);

        for(i = 0; msg_in->smi_data[i] != '\0'; i++)
            msg_out->smi_data[i] = toupper(msg_in->smi_data[i]);

        msg_out->smi_data[i] = '\0';
        smi_receive_release(sqp);
        smi_send_release(sqp);
    }
    smi_close(sqp);
    exit(EXIT_SUCCESS);
}
