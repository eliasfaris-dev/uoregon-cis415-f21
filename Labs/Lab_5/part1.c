#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "string_parser.h"

void main(int argc, char **argv){
	FILE* fp;
	fp = fopen(argv[2], "r");
	// Got from my Lab 1
	size_t length = 128;
	char* buf = malloc(length);
	char** commands;
	
	command_line first;
	command_line second;

	char** tokens;

	while(getline(&buf, &length, fp) != -1){
		first = str_filler(buf, ";");
		commands = first.command_list;
		for(int i = 0; commands[i] != NULL; i++){
			second = str_filler(commands[i], " ");
			tokens = second.command_list;
			if(strcmp(tokens[0], "exit") == 0){	
				fclose(fp);
				free(buf);
				free_commands(commands);
				free_commands(tokens);
				memset(&second, 0, 0);
				//free_commands(commands);
				memset(&first, 0, 0);
				return;
			}
			else{
				//call(tokens);
				free_commands(tokens);
				memset(&second, 0, 0);
			}
			
		}
		free_commands(commands);
		memset(&first, 0, 0);
	}
	free(buf);
	return;
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


void free_commands(char** command)
{
	for(int i = 0; command[i] != NULL; i++){
		free(command[i]);
	}

	free(command);
}