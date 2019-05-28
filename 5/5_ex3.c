#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

void sig_handler(int signum){
    char sig_str[32];
    sprintf(sig_str, " Received signal %d ", signum);
    psignal(signum, sig_str);
    // uncomment for ex3, comment for ex4
    // exit(0);
}

int main(int argc, char const *argv[]){
    struct sigaction s[34];   // for all possible signals (1-34)
    for(int i=0; i<34; ++i){
        s[i].sa_handler = sig_handler;
        s[i].sa_flags = 0;
        sigemptyset(&s[i].sa_mask);
        sigaction(i+1, &s[i], NULL);
    }
    while(1) pause();
    return 0;
}