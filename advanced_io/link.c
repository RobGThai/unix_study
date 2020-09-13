#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

int main(int argc, char **argv) {

    if(argc != 2){
        printf("Require a path to print information\n");
        return 1;
    }

    struct stat fileStat;
    
    if(stat(argv[1],&fileStat) < 0)    
        return 1;

    printf("Information for %s\n",argv[1]);
    printf("---------------------------\n");
    printf("File Size: \t\t%ld bytes\n",fileStat.st_size);
    printf("Number of Links: \t%lu\n",fileStat.st_nlink);
    printf("File inode: \t\t%lu\n",fileStat.st_ino);

    if(lstat(argv[1], &fileStat) < 0)
        return 1;

    printf("\n\nlstat info\n");
    printf("Device ID: \t\t %lu \n", fileStat.st_dev);

    printf("Mode: \t\t\t");

    if S_ISSOCK(fileStat.st_mode)
        printf("Socket \n");
    else if S_ISCHR(fileStat.st_mode)
        printf("Character Special File \n");
    else if S_ISDIR(fileStat.st_mode)
        printf("Directory \n");
    else if S_ISFIFO(fileStat.st_mode)
        printf("Un-named PIPE \n");
    else if S_ISLNK(fileStat.st_mode)
        printf("Symbolic Link \n");
    else if S_ISBLK(fileStat.st_mode)
        printf("Block Special File \n");
    else if S_ISREG(fileStat.st_mode)
        printf("Regular File \n");
    else
        printf("Unknown \n");

    printf("i-number: \t\t %lu \n", fileStat.st_ino);
    printf("User ID: \t\t %d \n", fileStat.st_uid);
    printf("Group ID: \t\t %d \n", fileStat.st_gid);
    printf("Size: \t\t\t  %ld bytes \n", fileStat.st_size);
    printf("Optimal Block Size: \t %ld bytes \n", fileStat.st_blksize);
    printf("Block Count: \t\t %ld blocks \n", fileStat.st_blocks);
    
    return 0;
}
