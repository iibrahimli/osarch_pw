#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 128

void copy(int fdsrc, int fddst){
    char buffer[BUF_SIZE];
    int rd;
    while((rd = read(fdsrc, buffer, BUF_SIZE)) > 0){
        write(fddst, buffer, rd);
    }
}

int main(int argc, char const *argv[]){

    int src = open("ex6.c", O_RDONLY);
    int dst = open("datte.txt", O_CREAT | O_WRONLY, 0666);

    copy(src, dst);

    close(src);
    close(dst);

    return 0;
}