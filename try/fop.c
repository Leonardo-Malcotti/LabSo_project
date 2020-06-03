#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <float.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <limits.h>
#include "projectLib.h"
//EISDR : valore di errno quando fopen cerca di aprire una cartella in w o r+
//
#define READ 0
#define WRITE 1

typedef struct stat Stat;
int main(int argn, char *argv[]){


    int ret=n_files_in_dir_subdir(argv[1]);
    printf("%d\n",ret);
    char **paths=(char **)malloc(ret*NAME_MAX*sizeof(char));
    int p=0;
    files_in_dir_subdir(argv[1],&p,paths);
    printf("%d\n",p);
    int i=0;
    for(i=0;i<p;i++){
        printf("%s\n",paths[i]);
    }
    //Stat buff;
/*
    printf("max child %d\n",CHILD_MAX);
    printf("max file %d\n",OPEN_MAX);
    printf("queue descriptors %ld\n",sysconf(_SC_MQ_OPEN_MAX));
    printf("max buf pipe %d\n",PIPE_BUF);
    */
/*
    int pip[2];
    pipe(pip);
    close(pip[READ]);
    int std= dup(STDOUT_FILENO);
//    printf("%d\n",std);
    dup2(pip[WRITE],STDOUT_FILENO);
    */
    /*
    char path[strlen(argv[1])+1];
    strcpy(path,argv[1]);
    int ret=stat(path,&buff);
    if(ret<0){
        printf("%s\n",strerror(errno));
    } else {
        printf("%d\n",buff.st_dev);
        printf("%llu\n",buff.st_ino);
        printf("%d\n",buff.st_mode);
        if(S_ISDIR(buff.st_mode)>0){
            printf("%d\n",S_ISDIR(buff.st_mode));
        } else {
            printf("%lld\n",buff.st_size);
            int tmp = open(path,O_RDWR);
            int len = lseek(tmp,0,SEEK_END);
            printf("%d\n",len);
        }
    }*/
    //printf("%d\n",system("ls test | wc -l"));

    /*fflush(NULL);
    close(pip[WRITE]);
    dup2(std,STDOUT_FILENO);
    printf("oki\n");
*/
    //FILE *f = fopen(path,"r+");
    /*
    if(f == NULL){
        //controlla se l'errore è dovuto al passaggio di una cartella
        if(errno == EISDIR){

            char command[10+strlen(path)];
            //inserisce il comando per cambiare cartella
            strcpy(command,"ls ");
            //specifica il path
            strcat(command,path);
            //stampa il contenuto
            //strcat(command," && ls");
            printf("%s\n",command);
            printf("%d\n",system(command));
        }
        return 0;
    }
    */

    //fclose(f);
    return 0;
}
