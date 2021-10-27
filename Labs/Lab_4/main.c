#include<stdio.h>
#include <sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

void script_print (pid_t* pid_ary, int size);

int main(int argc,char*argv[]){
    if (argc == 1){
        printf ("Wrong number of argument\n");
        exit (0);
    }
    
    /*
    * TODO
    * #1 declear child process pool
    * #2  spawn n new processes
    * first create the argument needed for the processes
    * for example "./iobound -seconds 10"
    * #3 call script_print
    * #4 wait for children processes to finish
    * #5 free any dynamic memories
    */



    freopen("log.txt", "w", stdout);
    char* arg_list[] = {"./iobound", "-seconds", "5", NULL};
    int num_proc = atoi(argv[1]);
    int count;
    int found = getpid();

    pid_t* pid_ary = (pid_t*)malloc(sizeof(pid_t) * num_proc);
    for(int i = 0; i < num_proc; i++){
        pid_ary[i] = fork();

        if(pid_ary[i] < 0){
            printf("Unable to declare child process"); 
        }

        if(found != getpid()){
        
            if(execvp("./iobound", arg_list) == -1){
                printf("New process couldn't be made\n");
            }

            exit(0);
        }
    }
    script_print(pid_ary, num_proc);

    for(int j = 0; j < num_proc; j++){
        waitpid(pid_ary[j], &count, 0);
    }

    return 0;
    free(pid_ary);
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
