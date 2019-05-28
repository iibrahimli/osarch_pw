#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

// returns 1 if fd is associated to a 
// character mode device, 0 otherwise
int my_isatty(int fd){
    struct stat fd_s;
    fstat(fd, &fd_s);
    if((fd_s.st_mode & S_IFMT) == S_IFCHR){
        return 1;
    }
    else return 0;
}

int main(int argc, char const *argv[]){

    if(argc != 2){
        fprintf(stderr, "Wrong argument count\n");
        exit(-1);
    }

    printf("%s : isatty=%d\n", argv[1], my_isatty(open(argv[1], O_RDONLY)));

    return 0;
}