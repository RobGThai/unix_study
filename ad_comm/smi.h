#include<stddef.h>
#include<stdbool.h>

struct smi_msg {
    long smi_mtype;
    struct client_id {
        long c_id1;
        long c_id2;
    } smi_client;
    char smi_data[1];
};

typedef void *SMIQ;
typedef enum {
    SMI_SERVER,
    SMI_CLIENT
} SMIENTITY;
#define SERVER_NAME_MAX 50

SMIQ *smi_open(
    const char *name,
    SMIENTITY entity,
    size_t msgsize
);

bool smi_close(
    SMIQ *sqp
);

bool smi_receive_getaddr(
    SMIQ *sqp,
    void **addr
);

bool smi_receive_release(
    SMIQ *sqp
);

bool smi_send_getaddr(
    SMIQ *sqp,
    struct client_id *client,
    void **addr
);

bool smi_send_release (
    SMIQ *sqp
);
