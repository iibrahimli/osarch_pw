#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


#define N 10
#define COUNT_MAX 10


struct t_arg {
    int  t_id;    // thread id
    int *last;    // each thread writes its t_id to this location after each increment
};


void *child_procedure(void *arg){
    int counter=0;

    for(; counter <= COUNT_MAX; ++counter){
        struct timespec t_sleep = {0, rand()};
        nanosleep(&t_sleep, NULL);
        printf("%d: %d\n", ((struct t_arg *)arg)->t_id, counter);
    }

    // set last to current thread id
    *(((struct t_arg *)arg)->last) = ((struct t_arg *)arg)->t_id;

    return NULL;
}


int main(int argc, char const *argv[]){

    srand(time(NULL));
    
    pthread_t  children[N];
    int        last_n;

    for(size_t i=0; i<N; ++i){
        struct t_arg *arg = malloc(sizeof *arg);
        arg->t_id = i+1;
        arg->last = &last_n;
        pthread_create(&children[i], NULL, child_procedure, (void *) arg);
    }
    
    for(size_t i=0; i<N; ++i){
        pthread_join(children[i], NULL);
    }

    printf("last: %d\n", last_n);

    return 0;
}