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
}

void call(tokens){
	// Do the command(list of tokens for 1 command)
}
