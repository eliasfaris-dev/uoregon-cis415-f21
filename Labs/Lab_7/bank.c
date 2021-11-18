#include <stdio.h>
#include <stdlib.h>
#include "account.h"
#include <pthread.h>
#include "string_parser.h"
#include <string.h>


int main(int argc, char** argv){
    if(argc == 2){
        FILE* fp;
        fp = fopen(argv[1], "r");

        command_line tokens;
        size_t length = 128;
        char* buf = malloc(length);
		//pthread_t thread;
		account index0;
		account index1;
		account index2;
		account index3;
		account index4;
		account index5;
		account index6;
		account index7;
		account index8;
		account index9;

		// 
        while((getline(&buf, &length, fp)) != -1){
            tokens = str_filler(buf, " ");
			if(strcmp(tokens.command_list[0], "D") == 0){

			}
			else if(strcmp(tokens.command_list[0], "T") == 0){

			}
			else if(strcmp(tokens.command_list[0], "C") == 0){

			}
			else if(strcmp(tokens.command_list[0], "W") == 0){

			}
        }




    }
    else{
        printf("Error! Only ./part1 <inputfile.txt>");
    }


}

void process_transaction(char** arg){


}

void update_balance(){

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
