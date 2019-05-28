#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int main(int argc, char const *argv[]){

    int ps_grep[2], grep_wc[2];

    pipe(ps_grep);
    pipe(grep_wc);

    if(fork() == 0){
        // ps eaux
        dup2(ps_grep[1], STDOUT_FILENO);

        close(ps_grep[0]);
        close(ps_grep[1]);
        close(grep_wc[0]);
        close(grep_wc[1]);

        execlp("ps", "ps", "eaux", NULL);
    }
    else{
        if(fork() == 0){
            // grep "$USER"
            
            dup2(ps_grep[0], STDIN_FILENO);
            dup2(grep_wc[1], STDOUT_FILENO);
            
            close(ps_grep[0]);
            close(ps_grep[1]);
            close(grep_wc[0]);
            close(grep_wc[1]);

            execlp("grep", "grep", (argc == 2) ? argv[1] : getenv("USER"), NULL);
        }
        else{
            // wc -l
            dup2(grep_wc[0], STDIN_FILENO);

            close(ps_grep[0]);
            close(ps_grep[1]);
            close(grep_wc[0]);
            close(grep_wc[1]);

            execlp("wc", "wc", "-l", NULL);
        }
    }

    return 0;
}