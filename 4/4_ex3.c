#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int pipe_cap = 0;

void signal_handler(int signum){
    printf("capacity: %d\n", pipe_cap);
    exit(0);
}

int main(int argc, char const *argv[]){
    int fd[2];
    pipe(fd);

    alarm(1);

    signal(SIGALRM, signal_handler);

    while(1){
        write(fd[1], "A", 1);
        ++pipe_cap;
        printf("\rcapacity: %d", pipe_cap);
    }

    return 0;
}