#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int main(int argn, char *argv[]){
    int i;//indice per i for
    int n,m;
    char input[];

    if(argn == 1){
        printf("argomento input non trovato\n");
        return 0;
    }
    if(argn>7){
        printf("troppi argomenti specificati\n");
        return 0;
    }
    for(i=1;i<argn;i+=2){
        if(i+1<=argn){
            printf("erroneo utilizzo degli argomenti");
            return 0;
        }
        if(strcmp(argv[i],"-s")){

            input[strlen(argv[i+1])+1];
            strcpy(input,argv[i+1]);
        }
        if(strcmp(argv[i],"-n")){
            char *err;
            long int tmp = strtol(argv[i+1],&err,10);
            if(*err){
                printf("erroneo utilizzo degli argomenti");
                return 0;
            }
            n=(int)tmp;
        }
        if(strcmp(argv[i],"-m")){
            char *err;
            long int tmp = strtol(argv[i+1],&err,10);
            if(*err){
                printf("erroneo utilizzo degli argomenti");
                return 0;
            }
            m=(int)tmp;
        }
    }

    //inorridisco ma intanto ok
    int caratteri[256];
    for(i=0;i<256;i++){caratteri[i]=0;}

    //char input[strlen(argv[1])+1];
    //strcpy(input,argv[1]);

    for(i=0; i<strlen(input);i++){
        caratteri[input[i]]++;
    }

    return 0;
}
