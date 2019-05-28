#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// message queue key
#define MSGQ_KEY 1
// message type
#define MSGQ_TYPE 1
// message data buffer size
#define MSGQ_BUFSIZE 64


// message buffer
struct msgbuf{
    long mtype;
    char mtext[MSGQ_BUFSIZE];
};


// handle error and exit
void handle(const char *where){
    fprintf(stderr, "Error in %s\n", where);
    exit(-1);
}


int main(int argc, char const *argv[]){
    
    pid_t cpid = fork();
    // create message queue
    int mq = msgget(MSGQ_KEY, IPC_CREAT | 0600);

    if(cpid == -1){
        // error
        handle("fork");
    }
    else if(cpid == 0){
        // child receives messages and counts bytes
        int bytes = 0;
        // get message queue handle
        int mrq = msgget(MSGQ_KEY, 0600);
        struct msgbuf rcv;
        int msg_len;
        // loop waits for messages
        while(1){
            if(msgrcv(mrq, (void *)&rcv, sizeof(rcv.mtext), MSGQ_TYPE, MSG_NOERROR) >= 0){
                msg_len = strlen(rcv.mtext);
                if(msg_len == 0){
                    printf("CHILD EXIT. total: %d bytes\n", bytes);
                    msgctl(mrq, IPC_RMID, NULL);
                    exit(0);
                }
                printf("child received (len %d): %s\n", msg_len, rcv.mtext);
                bytes += msg_len;
            }
        }
    }
    else{
        struct msgbuf m;
        m.mtype = 1;
        int rd;
        // parent sends messages from stdin
        while((rd = read(STDIN_FILENO, m.mtext, MSGQ_BUFSIZE)) > 0){
            m.mtext[rd-1] = '\0';
            if(msgsnd(mq, &m, sizeof(m.mtext), IPC_NOWAIT) == -1) handle("msgsnd");
            // printf("parent sent message with len %ld\n", strlen(m.mtext));
        }
        m.mtext[0] = '\0';
        if(msgsnd(mq, &m, sizeof(m.mtext), IPC_NOWAIT) == -1) handle("msgsnd");
        wait(NULL);
    }

    return 0;
}