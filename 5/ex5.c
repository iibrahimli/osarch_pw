#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void handle(){
    printf("message\n");
}

void handle_sigusr1(int signum){
    
}

int main(int argc, char const *argv[]){

    pid_t cpid;

    switch(cpid = fork()){
        case -1:
            // error
            printf("error on fork\n");
            exit(-1);
            break;
        case 0:
            // child
            printf("child: datte\n");
            break;
        default:
            // parent
            
    }

    return 0;
}