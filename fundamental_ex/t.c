#include <stdio.h>
#include <unistd.h>

int main() {
    int age;
    int *address = &age;
    *address = 37;
    printf("TEST\n");
    printf("%u %d - %p \n", *address, age, address);

    return 0;
}
