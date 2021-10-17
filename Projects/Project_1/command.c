#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "main.h"
#include "string_parser.h"
#include <sys/dir.h>

void listDir(){
    char buf[1048];
    char* cwd = (char*)malloc((1048 * sizeof(char)));
    DIR* dir;
    struct dirent* rdir;

    getcwd(cwd, 1048);
    dir = opendir(cwd);

    while((rdir = readdir(dir)) != NULL) {
        strcpy(buf, rdir->d_name);
        strcat(buf, " ");
        write(1, buf, strlen(buf));
    }
    strcpy(buf, "\n");
    write(1, buf, strlen(buf));
    closedir(dir);
    free(cwd);
}

void showCurrentDir(){

}

void makeDir(char *dirName){

}

void changeDir(char *dirName){

}

void copyFile(char *sourcePath, char *destinationPath){

}

void moveFile(char *sourcePath, char *destinationPath){

}

void deleteFile(char *filename){

}

void displayFile(char *filename){

}