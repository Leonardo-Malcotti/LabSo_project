#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "projectLib.h"

#define COUNT 1 //Quanti byte leggere ad ogni read
/*
prende in input un file .txt e ne stampa a video il report, fornendo statistiche.
il formato del file di input è:

ascii ricorrenza \n

*/

int main(int argc, char *argv[]) {
    int i;          //iteratore per cicli
    int finput;     //File Descriptor da inizializzare con il file in input
    int len_file;   //Lunghezza del file 'descritto' da finput
    int *buff;      //Buffer in cui salvo i dati letti

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
            controllo = 1;
            if((finput = open_file(argv[i+1],&len_file)) == -1){
                //stampa errori
                exit(-1);
            }
        }
    }

//Step 2: lettura da finput
    
    while(read(finput, buff, COUNT)>0){
        printf("%d\n", buff); //Per capire quanto legge, TEMPORANEO
    }


    

    close(finput);
    return 0;
}
