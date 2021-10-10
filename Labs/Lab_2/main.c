#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"

#define _GNU_SOURCE

int main(){

    char str[1048];

    printf(">>>");
    gets(str);

    while(str != "lfcat"){
        printf("Error: Unrecognized command!");
        printf(">>>");
        gets(str);
    }


}
