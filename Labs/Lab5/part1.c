#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "string_parser.h"

int main(int argc,char*argv[]){
    if (argc == 1){
        printf ("Wrong number of argument\n");
        exit (0);
    }

	int count;
	int n = -1;
    size_t length = 128;
	char* buf = malloc(length);
	FILE* fp = fopen(argv[1], "r");

	int found = getpid();
	command_line tokens;
	pid_t* pid_ary = (pid_t*)malloc(sizeof(pid_t) * 50);

    while(getline(&buf, &length, fp) != -1){
		tokens = str_filler(buf, " ");
		n++;
        pid_ary[n] = fork();

        if(pid_ary[n] < 0){
            printf("Unable to declare child process");
            free(pid_ary); 
        }
		printf(tokens.command_list[0]);
		printf(tokens.command_list);
		printf(tokens);
        if(found != getpid()){
        
            if(execvp(tokens.command_list[0], tokens.command_list) == -1){
                printf("New process couldn't be made\n");
                free(pid_ary);
            }

            exit(0);
        }
    }
    

    for(int j = 0; j < n; j++){
        waitpid(pid_ary[j], &count, 0);
    }

    free(pid_ary);
    fclose(fp);
    return 0;
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
