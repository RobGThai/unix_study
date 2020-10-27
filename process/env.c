#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int main(void) {
    int i;

    for(i = 0; environ[i] != NULL; i++) {
        printf("%s\n", environ[i]);
    }

    char *s;
    s = getenv("LOGNAME");

    if ( s == NULL)
        printf("variable not found \n");
    else
        printf("value is '%s'\n", s);

    return 0;
}
