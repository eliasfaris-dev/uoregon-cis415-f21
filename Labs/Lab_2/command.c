#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>


void lfcat(char* buf, long size){

    DIR* pdir;
    char* ptr;
    FILE* fp;
    ssize_t nread;
    size_t len = 0;
    char* line = NULL;

    struct dirent *dirp;

    //printf("<<In lfcat(): Step-01: Function called>>\n");
    if ((buf = (char *)malloc((size_t)size)) != NULL){
        ptr = getcwd(buf, (size_t) size);
        //printf("<<In lfcat(): Step-02: Listing all files in current dir.\n");
        //printf("CWD: %s\n", buf);
        pdir = opendir(buf);
        
        fp = freopen("output.txt", "w+", stdout);

        while((dirp = readdir(pdir)) != NULL){


            if((strcmp((dirp->d_name), "output.txt") != 0) && (strcmp((dirp->d_name), "main.c") != 0) && (strcmp((dirp->d_name), "command.c") != 0) && (strcmp((dirp->d_name), "command.h") != 0) && (strcmp((dirp->d_name), "main.o") != 0) && (strcmp((dirp->d_name), "Makefile") != 0) && (strcmp((dirp->d_name), ".") != 0) && (strcmp((dirp->d_name), "..") != 0) && (strcmp((dirp->d_name), "command.o") != 0) && (strcmp((dirp->d_name), "lab2.exe") != 0) && (strcmp((dirp->d_name), ".output.txt.swn") != 0) && (strcmp((dirp->d_name), ".output.txt.swo") != 0)){
                //printf("File: %s\n", dirp->d_name);

                write(1, "File: ", 6);
                write(1, dirp->d_name, strlen(dirp->d_name));
                write(1, "\n", 1);

                FILE* fp2 = fopen(dirp->d_name, "r");
       
                while((nread = getline(&line, &len, fp2)) != -1){

                    write(1, line, strlen(line));
                }

                //write(1, dirp->d_name, strlen(dirp->d_name));
                write(1, "\n", 1);
                write(1, "--------------------------------------------------------------------------------\n", 81);
        

                //closedir(pdir);
                fclose(fp2);
            }
        }


    }
    closedir(pdir);
    if(line != NULL){
        free(line);
    }
    free(buf);
    if(fp != NULL){
        fclose(fp);
    }

}