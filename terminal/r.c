#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>

int main() {
    ssize_t nread; 
    int max = 100;
    char *s = malloc(100);
    
    printf("FD: %i\n", STDIN_FILENO);
    nread = read(STDIN_FILENO, s, max - 1);
    printf("Err: %i\n", errno);
    printf("Read: %s\n Char: %ld\n", s, nread);

    free(s);
    return 0;
}
