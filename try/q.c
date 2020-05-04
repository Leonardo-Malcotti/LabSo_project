#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

/*
prende in input un file .txt e scrive un file qnm.txt
inserendo il numero di ricorrenze dei caratteri ascii
nel file di input, considera tutti e 256 i caratteri
il formato di output è:

ascii ricorrenza \n

*/

int main(int argn, char *argv[]){
    int i;//indice per i for
    int n = 0;
    int m = 0;
    FILE *finput = NULL;

    //primi controlli sugli argomenti
    if(argn == 1){
        printf("argomento input non trovato\n");
        return 0;
    }
    if(argn>7){
        printf("troppi argomenti specificati\n");
        return 0;
    }

    //
    //i controlli sugli argomenti sono migliorabili
    //

    for(i=1;i<argn;i+=2){

        //controlla che ci sia il numero giusto di argomenti
        if(i+1>=argn){
            //inserire istruzioni sul comando
            printf("erroneo utilizzo degli argomenti 1\n");
            return 0;
        }

        //apre il file indicato nei parametri
        //gestisce eventuali errori
        if(strcmp(argv[i],"-f")==0){

            finput = fopen(argv[i+1],"r+");

            if(finput == NULL){
                //
                //inserire controllo errori
                //
                printf("errore nel file passato\n");
            }
        }

        //legge dai parametri il valore di n
        //controllando che l'input si effettivamente un numero
        if(strcmp(argv[i],"-n") == 0){
            char *err;
            long int tmp = strtol(argv[i+1],&err,10);
            if(strcmp(err,"")!=0){
                //
                //inserire istruzioni sul comando
                //
                printf("erroneo utilizzo degli argomenti 2\n");
                return 0;
            }
            n=(int)tmp;
        }

        //legge dai parametri il valore di m
        //controllando che l'input si effettivamente un numero
        if(strcmp(argv[i],"-m") == 0){
            char *err;
            long int tmp = strtol(argv[i+1],&err,10);
            if(*err){
                //
                //inserire istruzioni sul comando
                //
                printf("erroneo utilizzo degli argomenti 3\n");
                return 0;
            }
            m=(int)tmp;
        }
    }

    //inorridisco ma intanto ok
    int caratteri[256]={[0 ... 255]=0};

    char rff;
    while(fscanf(finput,"%c",&rff)!=EOF){
        //printf("%c",input[i]);
        caratteri[rff]++;
    }

    fflush(finput);
    fclose(finput);

    //nome del file di output
    char outName[12];

    strcpy(outName,"q");
    sprintf(outName,"%s%d%d.txt",outName,n,m);

    //
    //controlli?
    //
    FILE *out = fopen(outName,"w+");

    for(i=0;i<256;i++){
        fprintf(out,"%d %d\n",i,caratteri[i]);
    }

    fflush(out);
    fclose(out);
    fflush(NULL);

    return 0;
}
