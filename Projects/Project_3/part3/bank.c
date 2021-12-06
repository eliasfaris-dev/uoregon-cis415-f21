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
pthread_t b_thread;
pthread_barrier_t bar;
account* the_acc;
int total_acc = 0;
char file;
command_line* tokens;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fixLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t fixCond = PTHREAD_COND_INITIALIZER;
int threadWaiting = 0;
int completed = 0;
int threadActive = 0;
int in = 0;

int main(int argc, char** argv){
	if(argc != 2){
		printf("Incorrect call of function");
	}
	else{
		
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
			
			pthread_barrier_init(&bar, NULL, (MAX_THREAD + 1));

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
				pthread_mutex_init(&the_acc[i].ac_lock, NULL);
			}
			
			//pthread_barrier_init(&bar, NULL, (MAX_THREAD + 1));
			tokens = malloc(sizeof(command_line) * 120000);

			while((getline(&buf, &size, fp)) != -1){
				tokens[in] = str_filler(buf, " ");
				in++;
			}
			int error;

			//threadActive = MAX_THREAD;
			for(int i = 0; i < total_acc; i++){
				int b = i * (in/ MAX_THREAD);
				error = pthread_create(&(tid[i]), NULL, &process_transaction, (void*) (tokens + b));
				if(error != 0){
					printf("Thread can't be created : [%s]\n", strerror(error));
				}
			}

			pthread_mutex_lock(&fixLock);
			pthread_barrier_wait(&bar);
			
			//threadActive = MAX_THREAD;
			// Might need to change this 
			threadActive = MAX_THREAD;
			error = pthread_create(&b_thread, NULL, &update_balance, NULL);
			//error = pthread_create(&b_thread, NULL, &update_balance, &update);
			if(error != 0){
				printf("Bank thread can't be created\n", strerror(error));
			}

			for(int i = 0; i < total_acc; i++){
				pthread_join(tid[i], NULL);
			}

			pthread_join(b_thread, NULL);

			for(int i = 0; i < total_acc; i++){
				printf("%d balance:  %0.2f\n", i, the_acc[i].balance);
			}

			/*
			for(int i = 0; i < index; i++){
				free_command_line(tokens[i]);
			}
			free_command_line(&tokens);
			*/
			fclose(fp);
			free(buf);
			//free(the_acc);
		}
	}
}
	
void process_transaction(void* arg){
	//pthread_barrier_wait(&bar);
	int threshold = in/MAX_THREAD;
	int curr = pthread_self();
	printf("In process\n");
	command_line* tokens = (command_line*)(arg);
	//printf("in completed %d\n", curr);
	pthread_barrier_wait(&bar);
	printf("completed: %d\n");
	for(int j = 0; j < threshold; j++){	
		//printf("%d\n", j);
		if(completed >= 5000){
			printf("in completed %d\n", curr);
			pthread_mutex_lock(&lock);
			threadWaiting++;
			if(threadActive == threadWaiting){
				pthread_mutex_lock(&fixLock);
				pthread_cond_signal(&fixCond);
				pthread_mutex_unlock(&fixLock);
			}
			pthread_cond_wait(&cond, &lock);
			pthread_mutex_unlock(&lock);

			printf("TEst\n");
		}
		
		if(strcmp(tokens[j].command_list[0], "C") == 0){
            for(int i = 0; i < total_acc; i++){
                if((strcmp(tokens[j].command_list[1], the_acc[i].account_number) == 0)){
                    if(strcmp(tokens[j].command_list[2], the_acc[i].password) == 0){
                        
						printf("C\n");
						
						
                    }
                }
            }
        }
		
		else if (strcmp(tokens[j].command_list[0], "D") == 0){
			double amount = atof(tokens[j].command_list[3]);
			for(int i = 0; i < total_acc; i++){
				if((strcmp(tokens[j].command_list[1], the_acc[i].account_number) == 0)){
                    if(strcmp(tokens[j].command_list[2], the_acc[i].password) == 0){
						pthread_mutex_lock(&the_acc[i].ac_lock);
                        the_acc[i].transaction_tracter += amount;
						the_acc[i].balance += amount;
						printf("D\n");
						pthread_mutex_unlock(&the_acc[i].ac_lock);
						pthread_mutex_lock(&lock);
						completed++;
						pthread_mutex_unlock(&lock);
                        
                    }
                }
			}
		}
		else if(strcmp(tokens[j].command_list[0], "W") == 0){
			double amount = atof(tokens[j].command_list[3]);
            for(int i = 0; i < total_acc; i++){
                if((strcmp(tokens[j].command_list[1], the_acc[i].account_number) == 0)){
                    if(strcmp(tokens[j].command_list[2], the_acc[i].password) == 0){
						pthread_mutex_lock(&the_acc[i].ac_lock);
                        the_acc[i].transaction_tracter += amount;
						the_acc[i].balance -= amount;
						printf("W\n");
						pthread_mutex_unlock(&the_acc[i].ac_lock);
						pthread_mutex_lock(&lock);
						completed++;
						pthread_mutex_unlock(&lock);
                        
                    }
                }
            }
		}

		else if(strcmp(tokens[j].command_list[0], "T") == 0){
			double amount = atof(tokens[j].command_list[4]);
            for(int i = 0; i < total_acc; i++){
                if((strcmp(tokens[j].command_list[1], the_acc[i].account_number) == 0)){
                    if(strcmp(tokens[j].command_list[2], the_acc[i].password) == 0){
                        for(int k = 0; k < total_acc; k++){
                            if(strcmp(tokens[j].command_list[3], the_acc[k].account_number) == 0){
								pthread_mutex_lock(&the_acc[i].ac_lock);
                                the_acc[i].balance -= amount;
                                the_acc[i].transaction_tracter += amount;
								printf("T\n");
								pthread_mutex_unlock(&the_acc[i].ac_lock);
								pthread_mutex_lock(&the_acc[k].ac_lock);
                                the_acc[k].balance += amount;
								
								pthread_mutex_unlock(&the_acc[k].ac_lock);

								pthread_mutex_lock(&lock);
								completed++;
								pthread_mutex_lock(&lock);
                                
                            }
                        }
                    }
                }
            }
		}
	}
	threadActive--;
	printf("active: %d\n", threadActive);
	printf("waiting: %d\n", threadWaiting);
	if(threadWaiting == threadActive){
		printf("testtt");
		pthread_mutex_lock(&fixLock);
		pthread_cond_signal(&fixCond);
		pthread_mutex_unlock(&fixLock);
	}

	pthread_exit(NULL);
	
}

void update_balance(void* arg){
	/*
	for(int i = 0; i < total_acc; i++){
		the_acc[i].balance += the_acc[i].reward_rate * the_acc[i].transaction_tracter;
	}
	*/
	printf("Got into update\n");
	while((completed > 0) || (threadActive > 0)){
		pthread_cond_wait(&fixCond, &fixLock);
		pthread_mutex_unlock(&fixLock);
		printf("Gets into update while");
		pthread_mutex_lock(&lock);
		for(int i = 0; i < total_acc; i++){
			the_acc[i].balance += the_acc[i].reward_rate * the_acc[i].transaction_tracter;
		}
		//(*((int*)arg))++;
		threadWaiting = 0;
		completed = 0;

		if(threadActive > 0){
			printf("thread active\n");
			pthread_cond_broadcast(&cond);
		}

		pthread_mutex_lock(&fixLock);
		pthread_mutex_unlock(&lock);
	}
	pthread_exit(NULL);
	
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

