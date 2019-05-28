#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// returns the environment variable named var
char *my_getenv(const char *var){
    char **en = __environ;
    char *val;
    int len = 0;

    // __environ is NULL-terminated
    for(; *en != NULL; ++en){
        // length of variable name (before =)
        len = strcspn(*en, "=");

        // if name matches our argument
        if(!strncmp(*en, var, len)){
            // copy the part after = to val
            val = malloc(strlen(*en) - len);
            strcpy(val, &(*en)[len+1]);

            return val;
        }
    }

    // given variable name not found in the array
    return NULL;
}

int main(int argc, char const *argv[]){

    if(argc != 2){
        fprintf(stderr, "Wrong argument count\n");
        exit(-1);
    }

    char *val = my_getenv(argv[1]);

    if(val)
        printf("%s\n", val);
    else
        printf("%s not found\n", argv[1]);
    
    return 0;
}