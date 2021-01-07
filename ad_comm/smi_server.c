#define SERVER_NAME "smsg_server"
#define DATA_SIZE 200
#define MAX_CLIENTS 20
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>
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

int main(void) {
    SMIQ *sqp;
    struct smi_msg *msg_in, *msg_out;
    int i;

    printf("server started MAX(%d)\n", SERVER_NAME_MAX);
    sqp =smi_open(SERVER_NAME, SMI_SERVER, DATA_SIZE);
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
