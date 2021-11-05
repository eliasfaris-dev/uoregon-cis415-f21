#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "string_parser.h"

int main(int argc, char** argv){
    if(argc == 2){
        filemode(argv);
    }
    else{
        printf("Error! Unsupported parameters to open file. Try ./part1 <filename>\n");
    }
}

void fileMode(char **argv){
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
			if(validate(tokens)){
				if(strcmp(tokens[0], "exit") == 0){	
					fclose(fp);
					free(buf);
					free_commands(commands);
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

int validate(char** tokens){
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
	char* element = tokens[0];
	if(strcmp(element, "ls") == 0){
		listDir();
	}
	else if(strcmp(element, "pwd") == 0){
		showCurrentDir();
	}
	else if(strcmp(element, "mkdir") == 0){
		makeDir(tokens[1]);
	}
	else if(strcmp(element, "cd") == 0){
		changeDir(tokens[1]);
	}
	else if(strcmp(element, "cp") == 0){
		copyFile(tokens[1], tokens[2]);
	}
	else if(strcmp(element, "mv") == 0){
		moveFile(tokens[1],tokens[2]);
	}
	else if(strcmp(element, "rm") == 0){
		deleteFile(tokens[1]);
	}
	else if(strcmp(element, "cat") == 0){
		displayFile(tokens[1]);
	}
}

int processes(char*argv[]){
    //FILE* fd = freopen("log.txt", "w", stdout);
    char* arg_list[] = {"./iobound", "-seconds", "5", NULL};
    int num_proc = atoi(argv[1]);
    int count;
    int found = getpid();

    pid_t* pid_ary = (pid_t*)malloc(sizeof(pid_t) * num_proc);
    for(int i = 0; i < num_proc; i++){
        pid_ary[i] = fork();

        if(pid_ary[i] < 0){
            printf("Unable to declare child process");
            free(pid_ary); 
        }

        if(found != getpid()){
        
            if(execvp("./iobound", arg_list) == -1){
                printf("New process couldn't be made\n");
                free(pid_ary);
            }
            exit(0);
        }
    }
    script_print(pid_ary, num_proc);
    for(int j = 0; j < num_proc; j++){
        waitpid(pid_ary[j], &count, 0);
    }
    free(pid_ary);
    //fclose(fd);
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