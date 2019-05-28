#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// my_malloc and my_free use a size_t variable
#define RESERVED_SIZE sizeof(size_t)


// initialises space before and after allocated
// segment with specific values, free checks
// them and reports error if they are modified
void *my_malloc(size_t size){

    // allocate memory for size + the 2 extra values
    void *ptr = malloc(size + 2*RESERVED_SIZE);
    
    if(!ptr){
        // return NULL pointer
        fprintf(stderr, "Error in my_malloc\n");
        return ptr;
    }

    // fixed value is the user given size value
    *((size_t *) ptr) = size;
    *((size_t *) ((char *)ptr + size + RESERVED_SIZE)) = size;

    return (char*) ptr + RESERVED_SIZE;
}


// frees the region pointed to by ptr, checks
// control values and reports if they're modified
// returns -1 if corruption is detected, 0 otherwise
int my_free(void *ptr){

    // check the control value which is before and
    // after the user specified memory region
    size_t first_ctl_val = *(size_t *)((char *)ptr-RESERVED_SIZE);

    // if the values do not match, there has been a corruption
    if( first_ctl_val != *(size_t *)((char *)ptr+first_ctl_val) ){
        fprintf(stderr, "Memory corruption\n");
        return -1;
    }

    // if the values match, the whole memory segment (user
    // specified size + 2 control values) is freed
    free((char*)ptr - RESERVED_SIZE);

    return 0;
}


int main(int argc, char const *argv[]){

    char *datte = my_malloc(5);
    
    datte[0] = 't';
    datte[1] = 's';
    datte[2] = 'u';
    datte[3] = 'g';
    datte[4] = 'a';
    // datte[5] = 'r';

    if(my_free(datte) == 0) printf("ok\n");

    return 0;
}