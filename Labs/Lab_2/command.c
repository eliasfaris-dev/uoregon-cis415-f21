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

    printf("<<In lfcat(): Step-01: Function called>>\n");
    if ((buf = (char *)malloc((size_t)size)) != NULL){
        ptr = getcwd(buf, (size_t) size);
        printf("<<In lfcat(): Step-02: Listing all files in current dir.\n");
        //printf("CWD: %s\n", buf);
        pdir = opendir(buf);
        
        fp = freopen("output.txt", "w+", stdout);

        while((dirp = readdir(pdir)) != NULL){


            if(strcmp(dirp->d_name, "output.txt") == 0){
                printf("File: %s\n", dirp->d_name);

                FILE* fp2 = fopen(dirp->d_name, "r");
       
                while((nread = getline(&line, &len, fp2)) != -1){

                    write(1, line, strlen(line));
                }

            //write(1, dirp->d_name, strlen(dirp->d_name));
                printf("--------------------------------------------------------------------------------\n");
        

            //closedir(pdir);
                fclose(fp2);
            }
        }


    }


}