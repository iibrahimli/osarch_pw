#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int my_system(const char *cmd){
    pid_t cpid;
    int status;

    switch(cpid = fork()){
        case -1:
            fprintf(stderr, "error on fork\n");
            return -1;

        case 0:
            // child: executes cmd
            execl("/bin/sh", "/bin/sh", "-c", cmd, NULL);

        default:
            // parent: waits for child
            waitpid(cpid, &status, 0);
            return WEXITSTATUS(status);
    }
}

int main(int argc, char const *argv[]){
    
    if(argc != 2){
        fprintf(stderr, "Wrong argument count\n");
        exit(-1);
    }

    return my_system(argv[1]);
}