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
#include <math.h>
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

void print_r_help(){
    printf("\n");
    printf("parametri\n\n");
    printf("-f  indica il percorso del file di input.\n");
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
    stat(arg,&buff);
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
    int contr = stat(arg,&buff);
    if(contr<0){
        //
        //controlli
        //
        return -1;
    } else {
        return S_ISDIR(buff.st_mode);
    }
}

int pipe_system_command(int pip[2],char *command){
    int contr = 0;
    contr=pipe(pip);
    if(contr<0){
        //
        //errori
        //
        return contr;
    }
    int std = dup(STDOUT_FILENO);
    if(std<0){
        //
        //errori
        //
        return std;
    }
    contr=dup2(pip[WRITE_P],STDOUT_FILENO);
    if(contr<0){
        //
        //errori
        //
        return contr;
    }
    contr=system(command);
    if(contr<0){
        //
        //errori
        //
        return contr;
    }
    fflush(NULL);
    contr=dup2(std,STDOUT_FILENO);
    if(contr<0){
        //
        //errori
        //
        return contr;
    }
    return 0;
}

int read_until_n(int des,char *buf, int *len){
    int rd=0;
    int c=0;
    do{
        char buf2[1];
        rd=read(des,buf2,1);
        if(*buf2=='\n'){
            *len=c;
            return 0;
        } else {
            strcat(buf,buf2);
            c++;
        }
    }while(rd>0);
    return -1;
}

int files_in_dir(char * path){
    //
    //fare controllo sulla dir?
    //
    int pip[2];
    char command[strlen(path)+11];//ls path | wc -l
    strcpy(command,"ls ");
    strcat(command,path);
    strcat(command," | wc -l");
    printf("%s\n",command);

    fflush(NULL);
    int contr = pipe_system_command(pip,command);

    if(contr<0){
        return -1;
    }
    
    close(pip[WRITE_P]);
    char buf[COSTANTE_LIMITE_TEMPORANEA];
    int len_buf;
    contr = read_until_n(pip[READ_P],buf,&len_buf);
    close(pip[READ_P]);

    printf("%s\n",buf);
    printf("%d\n",len_buf);
    printf("%lu\n",strlen(buf));
    double ret = 0,j=0;
    int i=0;
    for(i=strlen(buf);i>0;i--){
        printf("%c\n",buf[i]);
        int n=atoi(&buf[i]);
        if(n!=0){
            ret = ret + n*(pow(10,j));
        }
    }
    return (int)ret;
}
