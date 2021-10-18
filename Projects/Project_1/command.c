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
        write(1, "Error! Directory could not be created.\n", 39);
    }
}

void changeDir(char *dirName){
    int holder;

    holder = chdir(dirName);

    if(holder == -1){
        write(1, "Error! Directory could not be changed.\n", 39);
    }
}

void copyFile(char *sourcePath, char *destinationPath){

}

void moveFile(char *sourcePath, char *destinationPath){

}

void deleteFile(char *filename){
    int holder = 1;

    holder = remove(filename);

    if(holder == -1){
        write(2, "Error! Directory could not be removed.\n", 39);
    }
}

void displayFile(char *filename){
    char buf[1048];

    int file = open(filename, O_RDONLY);

    if(read(file, buf, 1048) < 0){
        write(1, "Error! Cannot read file.\n", strlen("Error! Cannot read file.\n"));
    }
    //write(1, "\n", strlen("\n"));
    write(2, buf, strlen(buf));
    //write(2, "\n", strlen("\n"));
    close(file);
}