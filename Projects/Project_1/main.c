#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "main.h"

#define _GNU_SOURCE

int main(int argc, char **argv){

    if(argc == 1){
        interactiveMode();
    }
    else if(argc == 3){
        fileMode();
    }

}

void interactiveMode(){

}

void fileMode(){

}
