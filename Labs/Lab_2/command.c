#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include <unistd.h>


void lfcat(char* buf, long size){


    char* ptr;
    printf("<<In lfcat(): Step-01: Function called>>\n");
    if ((buf = (char *)malloc((size_t)size)) != NULL){
        ptr = getcwd(buf, (size_t) size);
        printf("CWD: %s\n", buf);
    }


    printf("<<In lfcat(): Step-02: Listing all files in current dir.\n");

    printf(".\n");
    printf("..\n");

}