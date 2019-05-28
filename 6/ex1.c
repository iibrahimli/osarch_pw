#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>


int main(int argc, char const *argv[]){
    
    printf("int : %lu\n", sizeof(int));
    printf("char : %lu\n", sizeof(char));
    printf("char * : %lu\n", sizeof(char *));
    printf("struct{ char c; char *pc; int i;}: %lu\n",
    sizeof(struct{ char c; char *pc; int i;}));

    return 0;
}