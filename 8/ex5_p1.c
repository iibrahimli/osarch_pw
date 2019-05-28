// Sender process

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
#define SHM_SIZE 8192
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
    char mtext[MSGQ_BUFSIZE];  // acts as an int
};


// handle error and exit
void handle_error(const char *where){
    // fprintf(stderr, "Error in %s\n", where);
    perror(where);
    exit(-1);
}


int main(int argc, char const *argv[]){

    // get SHM_SIZE bytes of shared memory
    int shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0600);

    // attach to the created shared memory segment
    char *shm = shmat(shmid, NULL, 0);

    if(shm == (void *) -1) handle_error("shmat");

    // create message queue for later communication
    int smq = msgget(MSGQ_KEY, IPC_CREAT | 0600);
    if(smq == -1) handle_error("msgget");

    // initialize message buffer
    struct msgbuf snd;
    snd.mtype = MSG_TYPE;
    int rd;   // number of read bytes
    
    // p1 sends messages from stdin
    rd = read(STDIN_FILENO, shm, SHM_SIZE);

    // ignore the last newline (replace by '\0')
    --rd;

    // read does not null-terminate
    shm[rd] = '\0';

    // set message data field to number of bytes written to shm
    *(int *) snd.mtext = rd;
    if(msgsnd(smq, &snd, sizeof(snd.mtext), IPC_NOWAIT) == -1) handle_error("msgsnd");
    printf("sent notification\n");

    struct msgbuf rcv;

    // wait for confirmation
    if(msgrcv(smq, (void *)&rcv, sizeof(rcv.mtext), MSG_TYPE, MSG_NOERROR) >= 0){
        if(*(int *) rcv.mtext == rd){
            printf("received confirmation. freeing memory\n");
            // detach shared memory
            shmdt(shm);
            shmctl(shmid, IPC_RMID, NULL);
        }
    }

    return 0;
}