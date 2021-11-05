#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "string_parser.h"

int main(int argc, char*argv[]){
    /*
    char* arg_list[] = {"./iobound", "-seconds", "5", NULL};
    int num_proc = atoi(argv[1]);
    int count;
    int found = getpid();

    pid_t* pid_ary = (pid_t*)malloc(sizeof(pid_t) * num_proc);
    for(int i = 0; i < num_proc; i++){
        pid_ary[i] = fork();

        if(pid_ary[i] < 0){
            printf("Unable to declare child process");
            free(pid_ary); 
        }

        if(found != getpid()){
        
            if(execvp("./iobound", arg_list) == -1){
                printf("New process couldn't be made\n");
                free(pid_ary);
            }
            exit(0);
        }
    }
    script_print(pid_ary, num_proc);
    for(int j = 0; j < num_proc; j++){
        waitpid(pid_ary[j], &count, 0);
    }
    free(pid_ary);
    //fclose(fd);
    return 0;
    */


    
    if(argc != 2){
        printf("Error! You have the wrong number of paramaters\n");
    }
    else{
        int num_proc = atoi(argv[1]);
        fileMode(argv, num_proc);
    }

}

void fileMode(char **argv, int num_proc){
	FILE* fp;
	fp = fopen(argv[1], "r");
	// Got from my Lab 1
	size_t length = 128;
	char* buf = malloc(length);

	command_line first;
	char** tokens;

	while(getline(&buf, &length, fp) != -1){
		first = str_filler(buf, " ");
		tokens = first.command_list;
		call(tokens, num_proc);
		write(1, buf, strlen(buf));

		free_command_line(&first);
        memset(&tokens, 0, 0);
	}
	free(buf);
    fclose(fp);
    exit(-1);
	return;
}

void call(char** tokens, int num_proc){
    int found = getpid();

    pid_t* pid_ary = (pid_t*)malloc(sizeof(pid_t) * num_proc);
    for(int i = 0; i < num_proc; i++){
        pid_ary[i] = fork();

        if(pid_ary[i] < 0){
            printf("Unable to declare child process");
            free(pid_ary); 
        }

        if(found != getpid()){
        
            if(execvp(tokens[0], tokens) == -1){
                printf("New process couldn't be made\n");
                free(pid_ary);
            }
            exit(-1);
        }
    }
    int count;
    for(int j = 0; j < num_proc; j++){
        waitpid(pid_ary[j], &count, 0);
    }

    free(pid_ary);
    //printf("Successful\n");
}





int count_token (char* buf, const char* delim)
{
	if (buf == NULL){
		return EXIT_FAILURE;
	}

	int count = 0;

	char* copy = strdup(buf);

	char* token;
	char* saved;
	char* ptr;

	strtok_r(copy, "\n", &saved);

	for(ptr = copy;; ptr = NULL){
		token = strtok_r(ptr, delim, &saved);
		if(token != NULL){
			count++;
		}
		else{
			break;
		}
	}
	
	free(copy);
	return count + 1;

}

command_line str_filler (char* buf, const char* delim)
{
	command_line answer;
	
	int tokens = count_token(buf, delim);
	answer.num_token = tokens;

	answer.command_list = (char**)malloc(sizeof(char*) * answer.num_token);

	char* saved;
	char* ptr = buf;

	strtok_r(buf, "\n", &saved);

	int i = 0;
	for(ptr;; ptr = NULL, i++){
		char* tok = strtok_r(ptr,delim,&saved);
		if(tok != NULL){
			answer.command_list[i] = strdup(tok);
		}
		else{
			break;
		}
	}

	answer.command_list[(answer.num_token) - 1] = NULL;
	
	return answer;
}


void free_command_line(command_line* command)
{
	//TODO：
	/*
	*	#1.	free the array base num_token
	*/

	for(int i = 0; i < command->num_token; i++){
		free(command->command_list[i]);
	}

	free(command->command_list);
}
