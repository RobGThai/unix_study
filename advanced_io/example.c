#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int main() {

    char *current_path = NULL;
    long max_path;

    errno = 0;

    current_path = getcwd(current_path, 200);
    printf("Size of path(Dynamic): %lu \n", sizeof(current_path));
    printf("Current Path: %s \n", current_path);

    max_path = pathconf(current_path,_PC_PATH_MAX);
    printf("Max path length: %ld \n", max_path);

    return 0;
}
