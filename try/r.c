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
    char caratteri1 [33][4] = {"nul","soh","stx","etx","eot","enq","ack","bel","bs","ht","nl","vt","ff","cr","so","si","dle","dc1","dc2","dc3","dc4","nak","syn","etb","can","em","sub","esc","fs","gs","rs","us","sp"};
    int i,x;//indice per i for
    int finput,len_file;
    char buf[100];
    strcpy(buf,"");
    int contr,len;
    int j;
    int tmp;

    //apertura file
    if((finput=open_file("report.txt",&len_file))==-1){
                //stampa errori
        exit(-1);
    }
    //REPORT GENERALE
    //legge il file e stampa a mo' di tabella
    //a sinistra il valore ascii a destra la quantità e percentuale
    //intestazione
    printf("REPORT\n\nGeneral statistics:\n");
    printf("\033[0;31m");
    for(x=0;x<8;x++){
        printf("| char ");
        printf("  freq ");
        printf("     %% |");
    }
    printf("\033[0m");
    printf("\n");
  

    //legge il file e stampa a mo' di tabella
    //a sinistra il valore ascii a destra la quantità e percentuale
	for(i=0;i<16;i++){
        //divide in 8 colonne
        for(j=0;j<8;j++){
            //legge il primo valore, che è prima dello spazio
            contr = read_until_char(finput,' ',buf,&len);
            if(contr<0){
                //errore nella lettura
                exit(-1);
            }
            tmp=str_to_int(buf);
            if(tmp<33){
                printf("|%5s ",caratteri1[tmp]);
            }
            else if(tmp==127) {
                printf("|  del ");
            }
            else{
                 printf("|%5c ",tmp);
            }
            strcpy(buf,"");
            //legge il secondo valore, che è prima dello spazio
            contr = read_until_char(finput,' ',buf,&len);
            if(contr<0){
                //errore nella lettura
                exit(-1);
            }
            tmp=str_to_int(buf);
            printf(" %5d ",tmp);
            strcpy(buf,"");
            //legge il terzo valore (percentuale), che è prima della fine della riga
            contr = read_until_char(finput,'\n',buf,&len);
            if(contr<0){
                //errore nella lettura
                exit(-1);
            }
            printf(" %5.2f |",atof(buf));
            strcpy(buf,"");

        }
        printf("\n");
	}
    //stampa statistiche su categorie di caratteri
    printf("Categorie caratteri:\n");
    printf("\033[0;31m");
    printf("    categoria ");
    printf("  freq ");
    printf("      %% ");
    printf("\033[0m");
    printf("\n");
    for(j=0;j<8;j++){
        //legge il primo valore, che è prima dello spazio
        contr = read_until_char(finput,' ',buf,&len);
        if(contr<0){
            //errore nella lettura
            exit(-1);
        }
        printf("\033[0;32m");
        printf("%13s\t",buf);
        printf("\033[0m");
        strcpy(buf,"");
        contr = read_until_char(finput,' ',buf,&len);
        if(contr<0){
            //errore nella lettura
            exit(-1);
        }
        tmp=str_to_int(buf);
        printf(" %2d ",tmp);
        strcpy(buf,"");
        //legge il secondo valore, che è prima della fine della riga
        contr = read_until_char(finput,'\n',buf,&len);
        if(contr<0){
            //errore nella lettura
            exit(-1);
        }
        printf("\t%3.2f\n",atof(buf));
        strcpy(buf,"");

    }
    printf("\n");
    
    //REPORT PER FILE SPECIFICO SE RICHIESTO
    int boolean;
    int counter=0;
    if(argc>2 && strcmp(argv[1],"-f")==0){
        while(read_until_char(finput,'\n',buf,&len)!=-1)
        {
            i=2;
            boolean=1;
            while(boolean!=0 && i<argc)
            {
                if(strcmp(buf,argv[i])==0)
                {
                    counter++;
                    boolean=0;
                    printf("%s\n",argv[i]);
                    printf("\033[0;31m");
                    for(x=0;x<8;x++){
                        printf("| char ");
                        printf("  freq ");
                        printf("    %% |");
                    }
                    printf("\033[0m");
                    printf("\n");
                    for(i=0;i<16;i++){
                        //divide in 8 righe
                        for(j=0;j<8;j++){
                            //legge il primo valore, che è prima dello spazio
                            contr = read_until_char(finput,' ',buf,&len);
                            if(contr<0){
                                //errore nella lettura
                                exit(-1);
                            }
                            tmp=str_to_int(buf);
                            if(tmp<33){
                                printf("|%5s ",caratteri1[tmp]);
                            }
                            else if(tmp==127) {
                                printf("|  del ");
                            }
                            else{
                                printf("|%5c ",tmp);
                            }
                            strcpy(buf,"");
                            contr = read_until_char(finput,' ',buf,&len);
                            if(contr<0){
                                //errore nella lettura
                                exit(-1);
                            }
                            tmp=str_to_int(buf);
                            printf(" %5d ",tmp);
                            strcpy(buf,"");
                            contr = read_until_char(finput,'\n',buf,&len);
                            if(contr<0){
                                //errore nella lettura
                                exit(-1);
                            }
                            printf(" %5.2f |",atof(buf));
                            strcpy(buf,"");

                        }
                        printf("\n");
                    }
                    //stampa statistiche su categorie di caratteri
                    printf("Categorie caratteri:\n");
                    printf("\033[0;31m");
                    printf("    categoria ");
                    printf("  freq ");
                    printf("    %% ");
                    printf("\033[0m");
                    printf("\n");
                    for(j=0;j<8;j++){
                        //legge il primo valore, che è prima dello spazio
                        contr = read_until_char(finput,' ',buf,&len);
                        if(contr<0){
                            //errore nella lettura
                            exit(-1);
                        }
                        printf("\033[0;32m");
                        printf("%13s\t",buf);
                        printf("\033[0m");
                        strcpy(buf,"");
                        contr = read_until_char(finput,' ',buf,&len);
                        if(contr<0){
                            //errore nella lettura
                            exit(-1);
                        }
                        tmp=str_to_int(buf);
                        printf(" %2d ",tmp);
                        strcpy(buf,"");
                        contr = read_until_char(finput,'\n',buf,&len);
                        if(contr<0){
                            //errore nella lettura--
                            exit(-1);
                        }
                        printf("\t%3.2f\n",atof(buf));
                        strcpy(buf,"");
                    }
                    printf("\n");
                }
                i++;
            }
        }
        if(counter!=(argc-2)){
            printf("Non tutti i report dei file richiesti sono stati stampati. Verificare la correttezza del nome dei file.\n");
        }
    }
    else if(argc>=2 && strcmp(argv[1],"-f")!=0){
        printf("Parametri non inseriti correttamente. Avviare il report con i giusti parametri.\n");
    }
    close(finput);
    return 0;
}
