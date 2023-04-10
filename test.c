
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define is_token(c) (c=='|' || c=='<' || c=='>' || c=='&'|| c==' ')

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    pid_t pid;

    // create the pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // fork a child process
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // child process: execute "cat </dev/random" and write output to pipe
        close(pipefd[0]); // close unused read end of pipe
        dup2(pipefd[1], STDOUT_FILENO); // redirect stdout to pipe
        execl("/bin/cat", "cat", "/dev/random", NULL); // execute "cat </dev/random"
        perror("execl");
        exit(EXIT_FAILURE);
    } else {
       usleep(5);
	    // parent process
        int status;
        waitpid(pid, &status, 0); // wait for child process to finish
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            // child process exited successfully
            pid = fork();
            if (pid == -1) {
                perror("fork");
                exit(EXIT_FAILURE);
            } else if (pid == 0) {
                // grandchild process: execute "ls" and read input from pipe
                close(pipefd[1]); // close unused write end of pipe
                dup2(pipefd[0], STDIN_FILENO); // redirect stdin to pipe
                execl("/bin/ls", "ls", NULL); // execute "ls"
                perror("execl");
                exit(EXIT_FAILURE);
            } else {
                // parent process: wait for grandchild process to finish
                waitpid(pid, NULL, 0);
            }
        }
    }

    return 0;
}
