#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>

#define TYPE(b) ((statp->st_mode & (S_IFMT)) == (b))
#define MODE(b) ((statp->st_mode & (b)) == (b))

static void print_mode(const struct stat *statp) {
    if (TYPE(S_IFBLK))
        putchar('b');
    else if (TYPE(S_IFCHR))
        putchar('d');
    else if (TYPE(S_IFIFO))
        putchar('p');
    else if (TYPE(S_IFREG))
        putchar('-');
    else if (TYPE(S_IFLNK))
        putchar('l');
    else if (TYPE(S_IFSOCK))
        putchar('s');
    else
        putchar('?');
    
    putchar(MODE(S_IRUSR) ? 'r' : '-');
    putchar(MODE(S_IWUSR) ? 'w' : '-');
    if (MODE(S_ISUID)) {
        if(MODE(S_IXUSR))
            putchar('s');
        else
            putchar('S');
    } else if (MODE(S_IXUSR))
        putchar('x');
    else
        putchar('-');

    putchar(MODE(S_IRGRP) ? 'r' : '-');
    putchar(MODE(S_IWGRP) ? 'w' : '-');
    if (MODE(S_ISGID)) {
        if(MODE(S_IXGRP))
            putchar('s');
        else
            putchar('S');
    } else if (MODE(S_IXGRP))
        putchar('x');
    else
        putchar('-');

    putchar(MODE(S_IROTH) ? 'r' : '-');
    putchar(MODE(S_IWOTH) ? 'w' : '-');

    if (MODE(S_IFDIR) && MODE(S_ISVTX)) {
        if(MODE(S_IXOTH))
            putchar('t');
        else
            putchar('T');
    } else if (MODE(S_IXOTH))
        putchar('x');
    else
        putchar('-');
}

char *getGroupname(gid_t gid) {
    struct group *grp = getgrgid(gid);
    return grp->gr_name;
}

char *getUsername(uid_t uid) {
    struct passwd *pwd = getpwuid(uid);
    return pwd->pw_name;
}

char * getCurrentPath() {
    char *current_path = NULL;
    errno = 0;
    return getcwd(current_path, 200);
} 

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

    printf("Permission: \t\t");

    if((fileStat.st_mode & (S_IRGRP | S_IWGRP)) == (S_IRGRP|S_IWGRP))
        printf("Read and Write \n");
    else if((fileStat.st_mode & S_IRGRP) == S_IRGRP || (fileStat.st_mode & S_IWGRP) == S_IWGRP)
        printf("Read or Write \n");

    printf("i-number: \t\t %lu \n", fileStat.st_ino);
    printf("User ID: \t\t %s(%d) \n", getUsername(fileStat.st_uid), fileStat.st_uid);
    printf("Group ID: \t\t %s(%d) \n", getGroupname(fileStat.st_gid), fileStat.st_gid);
    printf("Size: \t\t\t  %ld bytes \n", fileStat.st_size);
    printf("Optimal Block Size: \t %ld bytes \n", fileStat.st_blksize);
    printf("Block Count: \t\t %ld blocks \n", fileStat.st_blocks);
 
    printf("------------------------------------------\n");
    print_mode(&fileStat);
    putchar('\n');

    printf("Path: \t\t\t %s \n", getCurrentPath());
    return 0;
}

