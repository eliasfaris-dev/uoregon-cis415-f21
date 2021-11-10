#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

int main(int argc, char* argv[]){
    FILE* fp;

    char* buf;
    size_t input;

    buf = (char*)malloc(sizeof(char) * 32));

    if(argc == 2){
        fp = fopen(argv[2], "r");
        if(fp == NULL){
            fprintf(stderr, "Couldn't open file");
            free(buf);
            exit(EXIT_FAILURE);
        }

        while((getline(&buf, &length, fp)) != -1){
		tokens = str_filler(buf, " ");
		//print_command_line(&tokens);
		n++;
        pid_ary[n] = fork();
        }

    }
    else{
        printf("Incorrect Usage");
    }
}