#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <float.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <limits.h>
#include "projectLib.h"

/*
prende in input un file .txt e scrive un file qnm.txt
inserendo il numero di ricorrenze dei caratteri ascii
nel file di input, considera tutti e 256 i caratteri
il formato di output è:

ascii ricorrenza \n

*/


int main(int argc, char *argv[]){
    int i, n, m, c = 0;
    int finput;
    int contr_arg[4] = {0,0,0,0};
    int len_file = 0;


    //primi controlli sugli argomenti
    if(argc == 1){
        printf("argomento input non trovato\n");
        print_q_help();
        exit(-1);
    }
    if(argc>9){
        printf("troppi argomenti specificati\n");
        print_q_help();
        exit(-1);
    }

    //controlla che ci sia il numero giusto di argomenti
    if(argc%2 == 0){
        //inserire istruzioni sul comando
        printf("erroneo utilizzo degli argomenti 1\n");
        print_q_help();
        exit(-1);
    }

    //
    //i controlli sugli argomenti sono migliorabili
    //

    for(i=1;i<argc;i+=2){

        //viene posto ad 1 se viene letto un parametro corretto
        int controllo = 0;

        //apre il file indicato nei parametri
        //gestisce eventuali errori
        if(param_check(argv[i],ARG_F,contr_arg) == 0){
            controllo = 1;
            if((finput = open_file(argv[i+1],&len_file)) == -1){
                //stampa errori
                printf("q: file non aperto\n");
                exit(-1);
            }
        }

        //lettura parametro -n con dovuti controlli
        if(param_check(argv[i],ARG_N,contr_arg) == 0){
            controllo = 1;
            if((n = str_to_int(argv[i+1])) == -1){
                //stampa errori
                exit(-1);
            }
        }

        //lettura parametro -m con dovuti controlli
        if(param_check(argv[i],ARG_M,contr_arg) == 0){
            controllo = 1;
            if((m = str_to_int(argv[i+1])) == -1){
                //stampa errori
                exit(-1);
            }

            if(m==0){
                printf("-m non può essere 0\n");
                exit(-1);
            }
        }

        //lettura parametro -c con dovuti controlli
        if(param_check(argv[i],ARG_C,contr_arg) == 0){
            controllo = 1;
            if((c=str_to_int(argv[i+1])) == -1){
                //stampa errori
                exit(-1);
            }

            if(c > m){
                printf("c deve essere un intero positivo minore di m\n");
                exit(-1);
            }
        }

        if(controllo == 0){
            printf("parametri non validi\n");
            exit(-1);
        }
    }

    //
    //Analisi del file
    //

    //inorridisco ma intanto ok
    int caratteri[256]={[0 ... 255]=0};

    char *rff = (char *)calloc(len_file,sizeof(char));
    int rd=0;

    int len_parti = (c==0)? len_file : len_file/m;

    len_parti = (len_parti<1)? 1 : len_parti;

    lseek(finput,((c==0)? 0 : (c-1)*len_parti),SEEK_SET);

    if(m==c){
        len_parti = len_file-(c-1)*len_parti;
    }

    rd = read(finput,rff,len_parti);
    /*
    if(rd<0){
        printf("read q: %s\n",strerror(errno));
    }
    else if(rd==0){
        printf("letto nulla\n");
    }
    else {
        printf("leggo %d\n",rd);
    }
    */
    //printf("pid: %d -> %s\n\n",getpid(),rff);
    for(i=0;i<rd;i++){
        caratteri[rff[i]]++;
    }
/*
    for(i=0;i<256;i++){
        printf("%d\n",caratteri[i]);
    }
    */
    free(rff);
    close(finput);

    //
    //scrittura del risultato sul canale d'uscita
    //
//if(rd>0){
    for(i=0;i<256;i++){
        char buff[100];
        strcpy(buff,"");
        sprintf(buff,"%d",caratteri[i]);
        //strcat(buff,"\n");
        write(PIPE_CHANNEL,buff,strlen(buff));
        write(PIPE_CHANNEL,"\n",1);
        //free(buff);
    }
//}
    close(PIPE_CHANNEL);

    return 0;
}
