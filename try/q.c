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
/*
prende in input un file .txt e scrive un file qnm.txt
inserendo il numero di ricorrenze dei caratteri ascii
nel file di input, considera tutti e 256 i caratteri
il formato di output è:

ascii ricorrenza \n

*/

void print_help() {
    printf("\n");
    printf("parametri\n\n");
    printf("-f  indica il percorso del file di input.\n");
    printf("-n  indica il numero del gruppo di file.\n");
    printf("    se non specificato n sarà 0 di default.\n");
    printf("-m  indica il numero di parti in cui il file è da suddividere.\n");
    printf("    se non specificato sarà 0, quindi verrà analizzato tutto il file.\n");
    printf("-c  indica quale parte analizzare, se a 0 analizza tutto il file.\n");
    printf("    se non specificato sarà 0.\n");
    printf("\n");
}

#define ARG_N 0
#define ARG_M 1
#define ARG_C 2
#define ARG_F 3

int main(int argn, char *argv[]){
    int i, n, m, c = 0;
    int finput;
    int contr_arg[4] = {0,0,0,0};
    int len_file = 0;


    //primi controlli sugli argomenti
    if(argn == 1){
        printf("argomento input non trovato\n");
        print_help();
        return 0;
    }
    if(argn>9){
        printf("troppi argomenti specificati\n");
        print_help();
        return 0;
    }

    //controlla che ci sia il numero giusto di argomenti
    if(argn%2 == 0){
        //inserire istruzioni sul comando
        printf("erroneo utilizzo degli argomenti 1\n");
        print_help();
        return 0;
    }

    //
    //i controlli sugli argomenti sono migliorabili
    //

    for(i=1;i<argn;i+=2){

        //apre il file indicato nei parametri
        //gestisce eventuali errori
        if(strcmp(argv[i],"-f")==0){

            //controlla che l'argomento non sia stato già usato
            if(contr_arg[ARG_F]!= 0){
                printf("hai usato -f troppe volte\n");
                print_help();
                return 0;
            } else {
                contr_arg[ARG_F] = 1;
            }

            finput = open(argv[i+1],O_RDWR);
            printf("file n %d\n",finput);
            if(finput < 0){
                //
                //inserire controllo errori
                //
                printf("errore nel file passato\n");
                print_help();
                return 0;
            }
            len_file = lseek(finput,0,SEEK_END);
            lseek(finput,0,SEEK_SET);
        }

        //legge dai parametri il valore di n
        //controllando che l'input si effettivamente un numero
        if(strcmp(argv[i],"-n") == 0){

            if(contr_arg[ARG_N]!= 0){
                printf("hai usato -n troppe volte\n");
                print_help();
                return 0;
            } else {
                contr_arg[ARG_N] = 1;
            }

            char *err;
            long int tmp = strtol(argv[i+1],&err,10);
            if(strcmp(err,"")!=0){
                //
                //inserire istruzioni sul comando
                //
                printf("erroneo utilizzo degli argomenti 2\n");
                print_help();
                return 0;
            }
            n=(int)tmp;
        }

        //legge dai parametri il valore di m
        //controllando che l'input si effettivamente un numero
        if(strcmp(argv[i],"-m") == 0){

            if(contr_arg[ARG_M]!= 0){
                printf("hai usato -m troppe volte\n");
                print_help();
                return 0;
            } else {
                contr_arg[ARG_M] = 1;
            }

            char *err;
            long int tmp = strtol(argv[i+1],&err,10);
            if(*err){
                //
                //inserire istruzioni sul comando
                //
                printf("erroneo utilizzo degli argomenti 3\n");
                print_help();
                return 0;
            }
            m=(int)tmp;
        }

        //legge dai parametri il valore di c
        //controllando che l'input si effettivamente un numero
        if(strcmp(argv[i],"-c") == 0){

            if(contr_arg[ARG_C]!= 0){
                printf("hai usato -c troppe volte\n");
                print_help();
                return 0;
            } else {
                contr_arg[ARG_C] = 1;
            }

            char *err;
            long int tmp = strtol(argv[i+1],&err,10);
            if(*err){
                //
                //inserire istruzioni sul comando
                //
                printf("erroneo utilizzo degli argomenti 4\n");
                print_help();
                return 0;
            }
            c=(int)tmp;

            if(c > m || c<0){
                printf("c deve essere un intero positivo minore di m\n");
                print_help();
                return 0;
            }
        }
    }

    //
    //Analisi del file
    //

    //inorridisco ma intanto ok
    int caratteri[256]={[0 ... 255]=0};

    char *rff = (char *)calloc(len_file,sizeof(char));

    int rd=0;


    if(c==0 || m==0){
        //legge tutto il file
        for(i=0;i<len_file;i++){
            rd = read(finput,rff,1);
            //printf("%d\n",rd);
            if(rd!=0){
                //printf("%c",rff[i]);
                caratteri[rff[i]]++;
            } else{
                //printf("fine\n");
            }
        }
    } else{
        int len_parti = len_file/m;
        len_parti=(len_parti<1)? 1: len_parti;

        lseek(finput,(c-1)*len_parti,SEEK_SET);

        rd = read(finput,rff,len_parti);

        if(rd>0){
            for(i=0;i<len_parti;i++){
                //printf("%c",rff[i]);
                caratteri[rff[i]]++;
            }
        }
    }


    //fflush(finput);

    close(finput);
/*
    //nome del file di output
    char outName[12];

    strcpy(outName,"q");
    sprintf(outName,"%s%d%d%d.txt",outName,n,m,c);
    //
    //controlli?
    //
    FILE *out = fopen(outName,"w+");
*/
    for(i=0;i<256;i++){
        char buff[sizeof(caratteri[i])];
        sprintf(buff,"%d",caratteri[i]);
        write(4,buff,sizeof(caratteri[i]));
        //printf("%d\n",caratteri[i]);
    }
    close(4);
    //fflush(out);
    //fclose(out);

    printf("fine %d\n",getpid());
    return 0;
}
