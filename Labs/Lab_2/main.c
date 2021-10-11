#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"

#define _GNU_SOURCE

int main(int argc, char *argv[]){

    char str[1048];

    //printf(">>> ");

    //while(fgets(str, 1048, stdin) != 'lfcat'){
    //    printf("Error: Unrecognized command!\n");
    //    printf(">>> ");
    //}

    while(1){
        printf(">>> ");
        fgets(str, 1048, stdin);
        if((strncmp(str, "lfcat", 5)) == 1){
            //break;
            return 0;
        }
        else{
            printf("Error: Unrecognized command!\n");
            continue;
        }
    }


}
