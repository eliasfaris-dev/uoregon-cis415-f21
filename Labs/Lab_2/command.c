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

    struct dirent *dirp;

    printf("<<In lfcat(): Step-01: Function called>>\n");
    if ((buf = (char *)malloc((size_t)size)) != NULL){
        ptr = getcwd(buf, (size_t) size);
        printf("CWD: %s\n", buf);
        pdir = opendir(buf);
        
        dirp = readdir(pdir);
        while(dirp != NULL){
            printf("%s\n", dirp->d_name);
        }

        closedir(pdir);
    }


    printf("<<In lfcat(): Step-02: Listing all files in current dir.\n");

}