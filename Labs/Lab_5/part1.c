#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "string_parser.h"

int main(int argc, char*argv[]){

    FILE* fw = stdout;  // output file
    FILE* fr = stdin;   // input file
    char *linebuf = NULL;   // read line buffer
    size_t linecap = 0;
    char write_buf[BUFSIZ]; // output write buffer
    command_line large_token_buffer;
	command_line small_token_buffer;
    int parent = getpid();
    pid_t pid_new;

    if ((argc == 3) && (strcmp(argv[1],"-f") == 0))
	{   // file mode
        //fw = freopen("output.txt", "w", stdout);
        fr = freopen(argv[2], "r", stdin);
	}
	else
	{   // error handling
		strcpy(write_buf, "Usage ./pseudo-shell -f <filename>\n");
		write(1, write_buf, strlen(write_buf));
		goto cleanup;
	}
	strcpy(write_buf, ">>> ");
    write(1, write_buf, strlen(write_buf));     // output initial prompt

    //loop until interactive exit or input command file ends
	while (getline (&linebuf, &linecap, fr) != -1)
	{
	    write(1, linebuf, strlen(linebuf));
		//tokenize line buffer
		//large token is seperated by ";"
		large_token_buffer = str_filler (linebuf, ";");

		//iterate through each large token
		for (int i = 0; large_token_buffer.command_list[i] != NULL; i++)
		{
			//tokenize large buffer
			//smaller token is seperated by " "(space bar)
			small_token_buffer = str_filler (large_token_buffer.command_list[i], " ");

            pid_new = fork();
            if(pid_new < 0){
                printf("Unable to declare child process");
                //free(pid_ary);
            }

            if (parent != getpid()) {

                if(execvp(small_token_buffer.command_list[0], small_token_buffer.command_list) == -1){
                    printf("New process couldn't be made\n");
                    //free(pid_ary);
                }

                exit(0);
            }

			//free smaller tokens and reset variable
			free_command_line(&small_token_buffer);
			memset (&small_token_buffer, 0, 0);
		}

		//free smaller tokens and reset variable
		free_command_line (&large_token_buffer);
		memset (&large_token_buffer, 0, 0);
        strcpy(write_buf, ">>> ");
        write(1, write_buf, strlen(write_buf));

	}

cleanup:
    //strcpy(write_buf, "Exiting pseudo-shell\n");
    //write(1, write_buf, 21);
    //fclose(fw);
    fclose(fr);
    free(linebuf);
    return 0;
}
































/*
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
*/

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