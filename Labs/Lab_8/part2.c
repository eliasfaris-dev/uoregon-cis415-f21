#include <stdio.h>
#include <stdlib.h>
#include "account.h"
#include <pthread.h>
#include "string_parser.h"
#include <string.h>
#define MAX_THREAD 10

void process_transaction();
void update_balance();
void printFunc();


pthread_t tid[MAX_THREAD];
account* the_acc;
int total_acc = 0;
char file;
command_line* tokens;

int main(int argc, char** argv){
	if(argc != 2){
		printf("Incorrect call of function");
	}
	else{
		//printf("Begenning of main\n");
		FILE* fp = fopen(argv[1], "r");
		if(fp == NULL){
			printf("File not found");
		}
		
		else{
			size_t size = 128;
			char* buf = (char*)malloc(size);
			getline(&buf, &size, fp);
			total_acc = atoi(buf);
			the_acc = malloc(sizeof(account) * total_acc);
			
			for(int i = 0; i < total_acc; i++){
				getline(&buf, &size, fp);
				buf[strcspn(buf, "\n")] = 0;
				strcpy(the_acc[i].index, buf);

				getline(&buf, &size, fp);
				buf[strcspn(buf, "\n")] = 0;
				strcpy(the_acc[i].account_number, buf);

				getline(&buf, &size, fp);
				buf[strcspn(buf, "\n")] = 0;
				strcpy(the_acc[i].password, buf);

				getline(&buf, &size, fp);
				buf[strcspn(buf, "\n")] = 0;
				the_acc[i].balance = atof(buf);

				getline(&buf, &size, fp);
				buf[strcspn(buf, "\n")] = 0;
				the_acc[i].reward_rate = atof(buf);

				the_acc[i].transaction_tracter = 0;

			}
			printf("Before tokenizing\n");
			tokens = malloc(sizeof(command_line) * 120000);
			printf("tokens malloced\n");
			int index = 0;
			while((getline(&buf, &size, fp)) != -1){
				tokens[index] = str_filler(buf, " ");
				index++;
				
			}

			printf("Before process_transaction\n");
			for(int i = 0; i < total_acc; i++){
				int b = i * (index/10);
				int error = pthread_create(&(tid[i]), NULL, &process_transaction, (void*) (tokens + b));
				if(error != 0){
					printf("Thread can't be created : [%s]\n", strerror(error));
				}
			}
			
			for(int i = 0; i < total_acc; i++){
				pthread_join(tid[i], NULL);
			}
			
			//printf("Before process_transaction\n");
			fclose(fp);
			free(buf);
			free(the_acc);
		}
	}
}
	

void process_transaction(void* arg){
	command_line* tokens = (command_line*)(arg);
	printf("Begenning process_transaction\n");
	for(int j = 0; j < 12000; j++){
		if(strcmp(tokens[j].command_list[0], "C") == 0){
            for(int i = 0; i < total_acc; i++){
                if((strcmp(tokens[j].command_list[1], the_acc[i].account_number) == 0)){
                    if(strcmp(tokens[j].command_list[2], the_acc[i].password) == 0){
                        break;
                    }
                }
            }
        }
		//printf("Before segfault\n");
		if (strcmp(tokens[j].command_list[0], "D") == 0){
			double amount = atof(tokens[j].command_list[3]);
			for(int i = 0; i < total_acc; i++){
				if((strcmp(tokens[j].command_list[1], the_acc[i].account_number) == 0)){
                    if(strcmp(tokens[j].command_list[2], the_acc[i].password) == 0){
                        the_acc[i].transaction_tracter += amount;
						the_acc[i].balance += amount;
                        break;
                    }
                }
			}
			//printf("After Deposit\n");
		}
		
	
	
		if(strcmp(tokens[j].command_list[0], "W") == 0){
			printf("Made it in W\n")
			double amount = atof(tokens[j].command_list[3]);
            for(int i = 0; i < total_acc; i++){
                if((strcmp(tokens[j].command_list[1], the_acc[i].account_number) == 0)){
                    if(strcmp(tokens[j].command_list[2], the_acc[i].password) == 0){
                        the_acc[i].transaction_tracter += amount;
						the_acc[i].balance -= amount;
                        break;
                    }
                }
            }
			//printf("After Withdraw\n");
		}

		if(strcmp(tokens[j].command_list[0], "T") == 0){
			double amount = atof(tokens[j].command_list[4]);
            for(int i = 0; i < total_acc; i++){
                if((strcmp(tokens[j].command_list[1], the_acc[i].account_number) == 0)){
                    if(strcmp(tokens[j].command_list[2], the_acc[i].password) == 0){
                        for(int k = 0; k < total_acc; k++){
                            if(strcmp(tokens[j].command_list[3], the_acc[k].account_number) == 0){
                                the_acc[i].balance -= amount;
                                the_acc[i].transaction_tracter += amount;
                                the_acc[k].balance += amount;
                                break;
                            }
                        }
                    }
                }
            }
			//printf("After Withdraw\n");
		}
	}
	//printf("After process transaction\n");
	free_command_line(&tokens);
	update_balance();
}

void update_balance(){
	for(int i = 0; i < total_acc; i++){
		the_acc[i].balance += the_acc[i].reward_rate * the_acc[i].transaction_tracter;
	}
	printFunc();
	
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

