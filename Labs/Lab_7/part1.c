#include <stdio.h>
#include <stdlib.h>
#include "account.h"
#include <pthread.h>
#include "string_parser.h"
#include <string.h>

account* the_acc;
int total_acc = 0;
char file;
command_line tokens;

int main(int argc, char** argv){
	if(argc != 2){
		printf("Incorrect call of function");
	}
	else{
		size_t len = 128;
		char* buf = malloc(len);
		FILE* fp = fopen(argv[1], "r");
		if(fp == NULL){
			printf("File not found");
		}
		else{
			fgets(buf, len, fp);
			total_acc = atoi(buf);
			file = argv[1];
			the_acc = malloc(sizeof(account) * total_acc);

			for(int i = 0; i < total_acc; i++){
				fgets(buf, len, fp);
				buf[strcspn(buf, "\n")] = 0;
				strcpy(the_acc[i].index, buf);

				fgets(buf, len, fp);
				buf[strcspn(buf, "\n")] = 0;
				strcpy(the_acc[i].account_number, buf);

				fgets(buf, len, fp);
				buf[strcspn(buf, "\n")] = 0;
				strcpy(the_acc[i].password, buf);

				fgets(buf, len, fp);
				buf[strcspn(buf, "\n")] = 0;
				the_acc[i].balance = atof(buf);

				fgets(buf, len, fp);
				buf[strcspn(buf, "\n")] = 0;
				the_acc[i].reward_rate = atof(buf);

				the_acc[i].transaction_tracter = 0;

			}
			// NEED TO FIGURE OUT WHAT TO PASS INTO PROCESS_TRANSACTION
			process_transaction();
		}
		
	}
}


void process_transaction(){
	size_t len = 128;
	char* buf = malloc(len);
	FILE* fp = fopen(file, "r");
	while((fgets(buf, len, fp)) != -1){
		tokens = str_filler(buf, " ");
		if(strcmp(tokens.command_list[0], "C") == 0){
            for(int i = 0; i < total_acc; i++){
                if((strcmp(tokens.command_list[1], the_acc[i].account_number) == 0)){
                    if(strcmp(tokens.command_list[2], the_acc[i].password) == 0){
                        break;
                    }
                }
            }
        }

		else if(tokens.command_list[0], "D"){
			double amount = atof(tokens.command_list[3]);
			for(int i = 0; i < total_acc; i++){
				if((strcmp(tokens.command_list[1], the_acc[i].account_number) == 0)){
                    if(strcmp(tokens.command_list[2], the_acc[i].password) == 0){
                        the_acc[i].transaction_tracter += amount;
						the_acc[i].balance += amount;
                        break;
                    }
                }
			}
		}

		else if(tokens.command_list[0], "W"){
			double amount = atof(tokens.command_list[3]);
            for(int i = 0; i < total_acc; i++){
                if((strcmp(tokens.command_list[1], the_acc[i].account_number) == 0)){
                    if(strcmp(tokens.command_list[2], the_acc[i].password) == 0){
                        the_acc[i].transaction_tracter += amount;
						the_acc[i].balance -= amount;
                        break;
                    }
                }
            }
		}

		else if(tokens.command_list[0], "T"){
			double amount = atof(tokens.command_list[4]);
            for(int i = 0; i < total_acc; i++){
                if((strcmp(tokens.command_list[1], the_acc[i].account_number) == 0)){
                    if(strcmp(tokens.command_list[2], the_acc[i].password) == 0){
                        for(int j = 0; j < total_acc; j++){
                            if(strcmp(tokens.command_list[3], the_acc[j].account_number) == 0){
                                the_acc[i].balance -= amount;
                                the_acc[i].transaction_tracter += amount;
                                the_acc[j].balance += amount;
                                break;
                            }
                        }
                    }
                }
            }
		}
	}

	update_balance();
}

void update_balance(){
	for(int i = 0; i < total_acc; i++){
		the_acc[i].balance += the_acc[i].reward_rate * the_acc[i].transaction_tracter;
	}
	
	free(the_acc);
}

void printFunc(){
	for(int i = 0; i < total_acc; i++){
		printf("%d balance:  %0.2f\n", i, the_acc[i].balance);
	}
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
