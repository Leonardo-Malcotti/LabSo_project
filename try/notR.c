#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
//#include "projectLib.h"

#define COUNT 1 //Quanti byte leggere ad ogni read
/*
prende in input un file .txt e ne stampa a video il report, fornendo statistiche.
il formato del file di input è:

ascii ricorrenza \n

*/
/*Visto che da problemi copio tutte le funzioni da projectLib.c e projectLib.h qui e tolgo l'import*/

#include <errno.h>
#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <limits.h>
#include <math.h>

#define ARG_F 2

char *arr_param[] = {"-n","-m","-f","-c"};


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
/*qui finiscono le cose importate temporaneamente*/

int main(int argc, char *argv[]) {
    int i;          //iteratore per cicli
    int finput;     //File Descriptor da inizializzare con il file in input
    int len_file;   //Lunghezza del file 'descritto' da finput
    char *buff;      //Buffer in cui salvo i dati letti

//Step 1: Gestione dell'argomento in ingresso e creazione del File Descriptor

    if(argc == 1){
        printf("argomento input non trovato\n");
        print_r_help();
        exit(-1);
    }
    if(argc > 3){
        printf("troppi argomenti specificati\n");
        print_r_help();
        exit(-1);
    }

    int contr_arg[4] = {1,1,0,1}; //Accetto solo un argomento di tipo -f
    for(i=1;i<argc;i++){
       //apre il file indicato nei parametri
        //gestisce eventuali errori
        if(param_check(argv[i],ARG_F,contr_arg) == 0){
            //controllo = 1;
            if((finput = open_file(argv[i+1],&len_file)) == -1){
                //stampa errori
                exit(-1);
            }
        }
    }

//Step 2: lettura da finput
	//printf("Starting the while cycle to read\n\tfile pointer n. %d\n\tfile length %d\n", finput, len_file);
    /*while(read(finput, buff, COUNT)>0){
        printf("%s\n", buff); //Per capire quanto legge, TEMPORANEO
    }*/
	//Proviamo con un for che ti devo dire
	for(i=0;i<len_file;i++){
		read(finput,buff,COUNT);
		printf("FOR CYCLE i=%d \t read byte: %s\n",i,buff);
	}

    close(finput);
    return 0;
}
