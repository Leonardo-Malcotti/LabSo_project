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
#include <sys/stat.h​>
#include <sys/types.h​>
#include <sys/wait.h​>
#include <sys/msg.h​>
#include <sys/ipc.h​>
#include "projectLib.h"


char *arr_param[] = {"-n","-m","-f","-c"};


void print_p_help(){
    printf("\n");
    printf("-f  indica il percorso del file di input.\n");
    printf("-n  indica il numero del gruppo di file.\n");
    printf("    se non specificato n sarà 0 di default.\n");
    printf("-m  indica il numero di parti in cui il file verrà suddiviso.\n");
    printf("    se non specificato sarà 0, quindi verrà analizzato tutto il file.\n");
    printf("\n");
}


void print_q_help(){
    printf("\n");
    printf("parametri\n\n");
    printf("-f  indica il percorso del file di input.\n");
    printf("-n  indica il numero del gruppo di file.\n");
    printf("    se non specificato n sarà 0 di default.\n");
    printf("-m  indica il numero di parti in cui il file è da suddividere.\n");
    printf("    deve essere un numero positivo diverso da 0, se non specificato darà errore\n");
    printf("-c  indica quale parte analizzare, se a 0 analizza tutto il file.\n");
    printf("    se non specificato sarà 0.\n");
    printf("\n");
}


int param_check(char *arg,int arg_type,int arr_check[]){
    if(strcmp(arg,arr_param[arg_type]) == 0){
        if(arr_check[arg_type]!= 0){
            printf("hai usato %s troppe volte\n",arr_param[arg_type]);
            //print_p_help();
            return -1;
        } else {
            arr_check[arg_type] = 1;
            return 0;
        }
    }
    return -1;
}


int str_to_int(char * arg){
    char *err;
    long int tmp = strtol(arg,&err,10);
    if(*err){
        printf("errore nella conversione della stringa\n");
        return -1;
    }
    if(tmp<0){
        printf("numero risultante negativo\n");
        return -1;
    }
    return (int)tmp;
}


int open_file(char * arg,int *len){
    struct stat buff;
    int tmp = open(arg,O_RDWR);
    test(arg,&buff);
    if(tmp < 0){
        //
        //inserire controllo errori
        //
        printf("errore nel file passato\n");
        return -1;
    }
    *len = (int)buff.st_size;
    return tmp;
}

int is_dir(char * arg){
    struct stat buff;
    int contr = test(arg,&buff);
    if(contr<0){
        //
        //controlli
        //
        return -1;
    } else {
        return S_ISDIR(buff.st_mode);
    }
}
