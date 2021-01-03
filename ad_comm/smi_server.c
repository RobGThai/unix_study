#define SERVER_NAME "smsg_server"
#define DATA_SIZE 200
#include "smi.h"
#include <stdlib.h>
#include <stdio.h>

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
