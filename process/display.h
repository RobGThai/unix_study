#include <sys/wait.h>
#include <signal.h>

void display_status(pid_t pid, int status);
void display_status_s(siginfo_t *infop); 
