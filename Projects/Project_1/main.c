#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "main.h"
#include "string_parser.h" 



#define _GNU_SOURCE

int main(int argc, char **argv){

    if(argc == 1){
        interactiveMode();
    }
    else if(inFileMode(argc, argv)){
       fileMode(argv[2]);
    }

}

void interactiveMode(int argc){
	if(argc == 1){
		char* buf = malloc(1048);
		char** commands;
		int length = 1048;
		command_line first;
		command_line second;
		size_t responce;
		char** tokens;

		while(1){
			write(1, ">>> ", 4);
			responce = getline(&buf,&length, stdin);
			first = str_filler(buf, ";");
               		commands = first.command_list;
	                for(int i = 0; commands[i] != NULL; i++){
                       		second = str_filler(*commands, " ");
                        	tokens = second.command_list;
                        	if(vaildate(tokens)){
                                	if(strcmp(tokens[0], "exit") == 0){
                                        	free(buf);
                                        	free_commands(tokens);
                                        	memset(&second, 0, 0);
                                        	free_commands(commands);
                                        	memset(&first, 0, 0);
                                       		 return;
                                	}
                                	else{
                                        call(tokens);
                                        free_commands(tokens);
                                        memset(&second, 0, 0);
                                	}
                        	}
                        	else{
                                	free_commands(tokens);
                                	memset(&second, 0, 0);
                                	break;
                        	}
                	}
                	free_commands(commands);
                	memset(&first, 0, 0);
        	}
        	free(buf);
        	return;

		
	}
	else{
		exit(EXIT_FAILURE);
	}
}

void fileMode(char **argv){
	FILE* fp;
	fp = fopen(argv[2], "r");
	
	FILE* out_fp;
	out_fp = fopen("output.txt", "w+");
	fclose(out_fp);
			
	// Got from my Lab 1
	
	char* buf = malloc(1048);
	char** commands;
	int length = 1048;
	command_line first;
	command_line second;

	char** tokens;

	while(getline(&buf, &length, fp) != -1){
		first = str_filler(buf, ";");
		commands = first.command_list;
		for(int i = 0; commands[i] != NULL; i++){
			second = str_filler(*commands, " ");
			tokens = second.command_list;
			if(vaildate(tokens)){
				if(strcmp(tokens[0], "exit") == 0){	
					fclose(fp);
					free(buf);
					free_commands(tokens);
					memset(&second, 0, 0);
					free_commands(commands);
					memset(&first, 0, 0);
					return;
				}
				else{
					call(tokens);
					free_commands(tokens);
					memset(&second, 0, 0);
				}
			}
			else{
				free_commands(tokens);
				memset(&second, 0, 0);
				break;
			}
		}
		free_commands(commands);
		memset(&first, 0, 0);
	}
	free(buf);
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


void free_commands(char** command)
{
	for(int i = 0; command[i] != NULL; i++){
		free(command[i]);
	}

	free(command);
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
			// Figure out a way to implement where this is printing in a file or on console depending on file mode or not
			printf("Error! Unsupported parameters for command: %s\n", tokens[0]);
			answer = 0;
		}
	}
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

void call(char** tokens){
	char element = tokens[0];
	if(strcmp(*element, "ls") == 0){
		listDir();
	}
	else if(strcmp(*element, "pwd") == 0){
		showCurrentDir();
	}
	else if(strcmp(*element, "mkdir") == 0){
		makeDir(tokens[1]);
	}
	else if(strcmp(*element, "cd") == 0){
		changeDir(tokens[1]);
	}
	else if(strcmp(*element, "cp") == 0){
		copyFile(tokens[1], tokens[2]);
	}
	else if(strcmp(*element, "mv") == 0){
		moveFile(tokens[1],tokens[2]);
	}
	else if(strcmp(*element, "rm") == 0){
		deleteFile(tokens[1]);
	}
	else if(strcmp(*element, "cat") == 0){
		displayFile(tokens[1]);
	}
}
