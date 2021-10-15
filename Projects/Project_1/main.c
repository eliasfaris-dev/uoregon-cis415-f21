#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "main.h"
#include "string_parser.h" //Header file from lab 1


#define _GNU_SOURCE

int main(int argc, char **argv){

    if(argc == 1){
        interactiveMode();
    }
    else if(inFileMode(argc, argv)){
       fileMode(argv[2]);
    }

}

void interactiveMode(){

}

void fileMode(char **argv){
	FILE* fp;
	fp = fopen(argv[2], "r");
	
	// Got from my Lab 1
	
	char* buf = malloc(1048);
	char** commands;

	command_line first;
	command_line second;

	char** tokens;

	while(getline(&buf, 1048, fp) != -1){
		first = str_filler(buf, ";");
		commands = first.command_list;
		for(int i = 0; first[i] != NULL; i++){
			second = str_filler(*first, " ");
			tokens = second.command_list;
			if(vaildate(tokens)){
				if(strcmp(tokens[0], "exit") == 0){
					free(commands);
					free(tokens);
					fclose(fp);
					free(buf);
					free_command_line(&second);
					memset(&second, 0, 0);
					free_command_line(&first);
					memset(&first, 0, 0);
					return;
				}
				else{
					call(tokens);
					free_command_line(&second);
					memset(&second, 0, 0);
				}
			}
			else{
				free_command_line(&second);
				memset(&second, 0, 0);
				break;
			}
		}
		free_command_line(&first);
		memset(&first, 0, 0);
	}
	free(buf);
	free(commands);
	free(tokens);
	fclose(fp);
	return;
}

int inFileMode(int argc, char **argv){
	int answer = 0;
	if(argc != 3){
		answer = 0;
	}
	else{
		if(strcmp(argv[1], "-f") == 0){
			FILE* fp;
			fp = fopen(argv[2], "r");
			if(fp != NULL){
				fclose(fp);
				answer = 1;
			}
			else{
				answer = 0;
			}
		}
		else{
			answer = 0;
		}
	
	}
	return answer;

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
	for(int i = 0; i < command->num_token; i++){
		free(command->command_list[i]);
	}

	free(command->command_list);
}

int vaildate(char** tokens){
	int answer = 0;
	int counter;
	for(int i = 0; tokens[i] != NULL; i++){
		counter++;
	}
	if(strcmp(tokens[0], "ls") == 0){
		if(counter == 1){
			answer =  1;
		}
		else{
			printf("Error! Unsupported parameters for command: %s\n", tokens[0]);
			answer = 0;
		}
	}
	// strcmp the rest of the chart here and make sure the count is possible. This will be a long code.
	// also check for exit and count should equal 1
	else if(strcmp(tokens[0], "pwd") == 0){
		if(counter == 1){
			answer = 1;
		}
		else{
			printf("Error! Unsupported parameters for command: %s\n", tokens[0]);
			answer = 0;
		}
	}

	else if(strcmp(tokens[0], "mkdir") == 0){
		if(counter == 2){
			answer = 1;
		}
		else{
			printf("Error! Unsupported parameters for command: %s\n", tokens[0]);
			answer = 0;
		}
	}

	else if(strcmp(tokens[0], "cd") == 0){
		if(counter == 2){
			answer = 1;
		}
		else{
			printf("Error! Unsupported parameters for command: %s\n", tokens[0]);
			answer = 0;
		}
	}

	else if(strcmp(tokens[0], "cp") == 0){
		if(counter == 3){
			answer = 1;
		}
		else{
			printf("Error! Unsupported parameters for command: %s\n", tokens[0]);
			answer = 0;
		}
	}

	else if(strcmp(tokens[0], "mv") == 0){
		if(counter == 3){
			answer = 1;
		}
		else{
			printf("Error! Unsupported parameters for command: %s\n", tokens[0]);
			answer = 0;
		}
	}

	else if(strcmp(tokens[0], "rm") == 0){
		if(counter == 2){
			answer = 1;
		}
		else{
			printf("Error! Unsupported parameters for command: %s\n", tokens[0]);
			answer = 0;
		}
	}

	else if(strcmp(tokens[0], "cat") == 0){
		if(counter == 2){
			answer = 1;
		}
		else{
			printf("Error! Unsupported parameters for command: %s\n", tokens[0]);
			answer = 0;
		}
	}
	// Check last 2 if/else checks
	else if(strcmp(tokens[0], "exit") == 0){
		if(counter == 1){
			answer = 1;
		}
		else{
			printf("Error! Unsupported parameters for command: %s\n", tokens[0]);
			answer = 0;
		}
	}

	else{
		printf("Error! Unrecognized command: %s\n", tokens[0]);
		answer = 0;
	}

	return answer;
}

void call(tokens){
	// Do the command(list of tokens for 1 command)
}
