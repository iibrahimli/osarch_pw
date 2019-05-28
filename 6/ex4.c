#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){

    if(argc != 2){
        fprintf(stderr, "Wrong argument count\n");
        exit(-1);
    }

    // getting the file size
    struct stat st;
    stat(argv[1], &st);
    int fsize = st.st_size;

    int fd = open(argv[1], O_RDONLY, 0);
    if(fd == -1){
        fprintf(stderr, "couldn't open file\n");
        return -1;
    }

    // map the whole file into memory
    char* data = mmap(NULL, fsize, PROT_READ, MAP_SHARED, fd, 0);
    if(data == MAP_FAILED){
        fprintf(stderr, "mmap failed\n");
        return -1;
    }

    // print the first line
    for(int i=0; data[i] != '\n'; ++i)
        printf("%c", data[i]);
    printf("\n");

    // print the last line
    int idx;
    for(idx=fsize-1; data[idx] != '\n'; --idx);
    ++idx;
    for(; idx < fsize; ++idx)
        printf("%c", data[idx]);
    printf("\n");

    munmap(data, fsize);

    return 0;
}