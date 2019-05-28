#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){

    int n = atoi(argv[1]);
    char *cmd = argv[2];

    char *args[argc];
    for(int a=2; a<argc; a++)
        args[a-2] = argv[a];
    args[argc-2] = malloc(15);
    args[argc-1] = NULL;

    int cpid;

    for(int i=0; i<n; ++i){
        cpid = fork();
        if(cpid == 0){
            sprintf(args[argc-2], "%d", i);
            // for(int i=0; i<argc-1; ++i)
            //     printf("args[%d] : %s\n", i, (args[i]) ? args[i] : "NULL");
            execvp(cmd, args);
        }
    }

    int ret, rcode = 0;

    for(int j=0; j<n; ++j){
        wait(&ret);
        if(WEXITSTATUS(ret) != 0) rcode = 1;
    }

    return rcode;
}