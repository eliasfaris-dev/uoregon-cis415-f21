#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"

#define _GNU_SOURCE

int main(int argc, char *argv[]){

    char str[1048];

    while(1){
        printf(">>> ");
        fgets(str, 1048, stdin);
        if((strncmp(str, "lfcat", 5)) == 0){
            break;    
        }
        else{
            printf("Error: Unrecognized command!\n");
            continue;
        }
    }

    lfcat();





}
