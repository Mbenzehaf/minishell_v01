
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void sigint_handler(int signal) {
   // printf("Caught SIGINT signal, exiting...\n");
    //exit(1);
    printf("\n");
}
void sigquit_handler(int signal) {
   // printf("Caught SIGQUIT signal, exiting...\n");
    exit(1);
}
int main() {
   char *str;
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    while (1)
    {
        str=readline(">");
        printf("%s\n",str);
        free(str);
    }
    
    return 0;
}