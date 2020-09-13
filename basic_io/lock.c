#define LOCKDIR "/tmp/"
#define MAXTRIES 10
#define NAPLENGTH 2
#include <stdio.h>
#include "../common/ec.h"

static char *lockpath(char *name) {
    static char path[100];
    if (snprintf(path, sizeof(path), "%s%s", LOCKDIR, name) > sizeof(path))
        return NULL;
    return path;
}

bool lock(char *name) {
    char *path;
    int fd, tries;

    ec_null( path = lockpath(name) )
    tries = 0;

}

int main() {

    return 0;
}
