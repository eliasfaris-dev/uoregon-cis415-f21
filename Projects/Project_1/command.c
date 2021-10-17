#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
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
    
    char* ptr = NULL;

    long len = pathconf(".", _PC_PATH_MAX);
    char buf[len];
    ptr = getcwd(buf, len);

    write(1, ptr, strlen(ptr));
    write(1, "\n", 1);

}

void makeDir(char *dirName){
    int holder;

    holder = mkdir(dirName, 0777);

    if(holder == -1){
        write(2, "Error! directory could not be created.\n", 39);
    }
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