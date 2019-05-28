// Receiver process

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// size of shared memory segment
// (in reality will be closest multiple of page size)
#define SHM_SIZE 8196
// shared memory key
#define SHM_KEY 1

// message queue key
#define MSGQ_KEY 1
// message type
#define MSG_TYPE 1
// message data buffer size
#define MSGQ_BUFSIZE sizeof(int)


// message buffer
struct msgbuf{
    long mtype;
    char mtext[MSGQ_BUFSIZE];  // treated as an int
};


// handle error and exit
void handle(const char *where){
    fprintf(stderr, "Error in %s\n", where);
    exit(-1);
}


int main(int argc, char const *argv[]){

    // get SHM_SIZE bytes of shared memory
    int shmid = shmget(SHM_KEY, 0, SHM_RDONLY);
    
    // attach to the created shared memory segment
    char *shm = shmat(shmid, NULL, 0);

    // get message queue handle
    int rmq = msgget(MSGQ_KEY, 0600);
    if(rmq == -1) handle("msgget");
    
    // initialize receiving message buffer
    struct msgbuf rcv;

    // initialize confirmation message buffer
    struct msgbuf conf;
    conf.mtype = MSG_TYPE;

    // wait for message
    while(1){
        // waits for message with number of bytes
        if(msgrcv(rmq, (void *)&rcv, sizeof(rcv.mtext), MSG_TYPE, MSG_NOERROR) >= 0){

            // print shared memory segment
            printf("received %d bytes: %s\n", *(int *) rcv.mtext, shm);

            // detach shared memory
            shmdt(shm);

            // respond with confirmation
            *(int *) conf.mtext = *(int *) rcv.mtext;
            if(msgsnd(rmq, &conf, sizeof(conf.mtext), IPC_NOWAIT) == -1) handle("msgsnd");
            printf("sent confirmation\n");

            // destruct the msg queue
            msgctl(rmq, IPC_RMID, NULL);

            break;
        }
    }


    return 0;
}