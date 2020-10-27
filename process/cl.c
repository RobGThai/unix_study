/* Command-line application */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 200
#define MAXARG 20

extern char **environ;

void error(void) {
    fprintf(stderr, "Error reading commands"); 
}

void set(int argc, char *argv[]) {
    int i;

    if (argc != 1)
        printf("Extra args\n");
    else
        for(i = 0; environ[i] != NULL; i++)
            printf("%s\n", environ[i]);
}

void asg(int argc, char *argv[]) {
    char *name, *val;
    
    if (argc != 1)
        printf("Extra args\n");
    else {
        name = strtok(argv[0], "=");
        val = strtok(NULL, "");
        if(name == NULL || val == NULL)
            printf("bad command\n");
        else
            setenv(name, val, true);
    }
}

static bool getargs(int *argcp, char *argv[], int max, bool *eofp) {
    static char cmd[MAXLINE];
    char *cmdp;
    int i;

    *eofp = false;

    if(fgets(cmd, sizeof(cmd), stdin) == NULL) {
        if(ferror(stdin))
           error(); 

        *eofp = true;
        return false;
    }

    if(strchr(cmd, '\n') == NULL) {
        /* eat up rest of line */
        while(true) {
            switch( getchar()) {
                case '\n':
                    break;
                case EOF:
                    if (ferror(stdin))
                        error();
                default:
                    continue;
            }
            break;
        }
        printf("Line too long -- command ignored\n");
        return false;
    }
    
    cmdp = cmd;
    for(i = 0; i < max; i++) {
        if((argv[i] = strtok(cmdp, " \t\n")) == NULL)
            break;
        cmdp = NULL;
    }

    if(i >= max) {
        printf("Too many args -- command ignored\n");
        return false;
    }
    *argcp = i;
    return true;
}

static void execute(int argc, char *argv[]) {
    execvp(argv[0], argv);
    printf("Can't execute\n");
}

int main(void){
    char *argv[MAXARG];
    int argc;
    bool eof;

    while(true) {
        printf("@ ");
        if(getargs(&argc, argv, MAXARG, &eof) && argc > 0) {
            if(strchr(argv[0], '=') != NULL)
                asg(argc, argv);
            else if(strcmp(argv[0], "set") == 0)
                set(argc, argv);
            else
                execute(argc, argv);
        }

        if(eof)
            exit(EXIT_SUCCESS);
    }

    return 0;
}
