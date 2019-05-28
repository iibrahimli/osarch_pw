#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define REL_ADDR(s, c) (void *)(((void *) &(s.c) - (void *) &s))

struct s {
    char c;
    char *pc;
    int i;
    char d;
};

int main(int argc, char const *argv[]){
    
    struct s v;
    printf("s.c : %p\n", REL_ADDR(v, c));
    printf("s.pc: %p\n", REL_ADDR(v, pc));
    printf("s.i : %p\n", REL_ADDR(v, i));
    printf("s.i : %p\n", REL_ADDR(v, d));

    return 0;
}