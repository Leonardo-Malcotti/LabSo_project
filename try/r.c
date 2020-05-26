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

int main(int argc, char *argv[]) {
    int i;//indice per i for
    int finput,len_file;

    //primi controlli sui parametri
    if(argc != 3){
        printf("utilizzo erroneo dei parametri\n");
        print_r_help();
        exit(-1);
    }

    //r accetta solo il parametro -f, con un valore che deve essere un file
    if(strcmp(argv[1],"-f") == 0){
        if((finput=open_file(argv[2],&len_file))==-1){
            //stampa errori
            exit(-1);
        }
    }

    //legge il file e stampa a mo' di tabella
    //a sinistra il valore ascii a destra la quantità
    //i valori letti vengono convertiti in int perchè hanno una formattazione più ordinata
	for(i=0;i<len_file;i+=0){
		char buf[len_file];
        strcpy(buf,"");
        int contr,len;
        int j;
        int tmp;

        //divide in 8 righe
        for(j=0;j<8;j++){
            //legge il primo valore, che è prima dello spazio
            contr = read_until_char(finput,' ',buf,&len);
            if(contr<0){
                //errore nella lettura
                exit(-1);
            }
            i+=len+1;
            tmp=str_to_int(buf);
            printf(" |%5d ",tmp);
            strcpy(buf,"");
            //legge il secondo valore, che è prima della fine della riga
            contr = read_until_char(finput,'\n',buf,&len);
            if(contr<0){
                //errore nella lettura
                exit(-1);
            }
            i+=len+1;
            tmp=str_to_int(buf);
            printf(" %5d| ",tmp);
            strcpy(buf,"");
        }
        printf("\n");
	}

    close(finput);
    return 0;
}
