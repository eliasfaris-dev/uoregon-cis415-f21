#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "string_parser.h"
#include <signal.h>

void script_print();


int main(int argc,char*argv[]){
    if (argc == 1){
        printf ("Wrong number of argument\n");
        exit (0);
    }

    
    sigset_t signal_set;
    sigemptyset(&signal_set);
    sigaddset(&signal_set, SIGUSR1);
    sigaddset(&signal_set, SIGALRM);
    sigprocmask(SIG_BLOCK, &signal_set, NULL);

	int count;
	int n = -1;
    size_t length = 128;
	char* buf = malloc(length);
	FILE* fp = fopen(argv[1], "r");
    int signal;

	int found = getpid();
	command_line tokens;
	pid_t* pid_ary = (pid_t*)malloc(sizeof(pid_t) * 100);

    while((getline(&buf, &length, fp)) != -1){
		tokens = str_filler(buf, " ");
		print_command_line(&tokens);
		n++;
        pid_ary[n] = fork();

		
        if(found != getpid()){
            
            int answer = sigwait(&signal_set, &signal);

            if(execvp(tokens.command_list[0], tokens.command_list) == -1){
                printf("New process couldn't be made\n");
                free(pid_ary);
                free(fp);
				free(buf);
            }

            exit(0);
        }
        
    }
    
    //script_print(pid_ary, n);
    //sleep(2);
    //for(int i = 0; i < n; i++){
        //kill(pid_ary[i], SIGUSR1);
    //}

    kill(0, SIGUSR1);

    for(int i = 0; i < n; i++){
        kill(pid_ary[i], SIGSTOP);
    }

    //sleep(2);

    for(int i = 0; i < n; i++){
        kill(pid_ary[i], SIGCONT);
    }

    for(int i = 0; i < n; i++){
        waitpid(pid_ary[i], &count, 0);
    }
	free(buf);
    free(pid_ary);
    fclose(fp);
	free_command_line(&tokens);
    return 0;
}

void script_print (pid_t* pid_ary, int size){
    FILE* fout;
    fout = fopen ("top_script.sh", "w");
    fprintf(fout, "#!/bin/bash\ntop");
    for (int i = 0; i < size; i++){
        fprintf(fout, " -p %d", (int)(pid_ary[i]));
    }
    fprintf(fout, "\n");
    fclose (fout);

    char* top_arg[] = {"gnome-terminal", "--", "bash", "top_script.sh", NULL};
    pid_t top_pid;

    top_pid = fork();
    {
        if (top_pid == 0)
        {
            if(execvp(top_arg[0], top_arg) == -1)
            {
                perror ("top command: ");
            }
            exit(0);
        }
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
	

	// Mem leaks cus of strdup
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
			// For some reason I have to make a copy of buf. It "Aborted (core dumped) without the strdup, I am not sure why."
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

void print_command_line(command_line* command){
	for(int i = 0; i < command->num_token; i++){
		printf("%dth: '%s'\n", i, command->command_list[i]);
	}
}
