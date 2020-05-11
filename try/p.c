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
#include "projectLib.h"
/*
o si trova il modo di rendere -m e -n obbligatori
o si trova il modo di non dover specificare la lunghezza di paths
*/





int main(int argc, char *argv[]) {
    int i;//indice per i for
    int n= 0;
    int m= 0;

    //primi controlli sugli argomenti
    //i parametri dovrebbero essere almeno 7:
    //1 per il nome dell'eseguibile
    //4 per n,m e i loro valori
    //1 per -f
    //poi almeno 1 per inizializzare f
    if(argc < 7){
        printf("argomento input non trovato\n");
        return 0;
    }

    char * paths[argc-6];//numero di files specificati
    int contr_arg[3] = {0,0,0};

    //controlli e lettura dei parametri
    for(i=1;i<argc;i++){
        int controllo = 0;
        //legge dai parametri il valore di n
        //controllando che l'input si effettivamente un numero
        if(param_check(argv[i],ARG_N,contr_arg) == 0){

            //segnala che è stato letto un parametro valido
            controllo =1;

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
            i++;
        }

        //legge dai parametri il valore di m
        //controllando che l'input si effettivamente un numero
        if(param_check(argv[i],ARG_M,contr_arg) == 0){

            //segnala che è stato letto un parametro valido
            controllo =1;

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
            i++;
        }

        //legge i valori del parametro -f
        if(param_check(argv[i],ARG_F,contr_arg) == 0){

            //segnala che è stato letto un parametro valido
            controllo =1;

            //legge tutti i file specificati

            //
            //magari sarebbe bene da aggiungere qualche controllo sulla correttezza degli stessi
            //
            int ct = 0;
            while(i+1 < argc && argv[i+1][0]!='-'){
                //char tmp[strlen(argv[i+1])];
                paths[ct] = argv[i+1];
                i++;
                ct++;
            }
        }

        if(controllo == 0){
            printf("parametri non validi\n");
            return 0;
        }
    }

    //fa un fork per ogni file
    int pid= -1;
    int tmp_m=1;
    int pipes[(argc-6)*m][2];
    int map_pipes[(argc-6)*m];
    int c_pipes =0;

    for(i=0; i<argc-6 && pid!=0;i++){
        tmp_m=1;
        //printf("%s\n",paths[i]);
        while(pid!=0 && tmp_m<=m){

            pipe(pipes[c_pipes]);
            printf("pipe %d creata\n",c_pipes);
            pid = fork();

            if(pid!=0){
                map_pipes[c_pipes]=pid;
                c_pipes++;
                tmp_m++;
            }
        }
    }
    //printf("%d %d %d\n",n,i,tmp_m);

    int caratteri[256]={[0 ... 255]=0};

    if(pid!=0){
        for(i=0; i<c_pipes ;i++){
            close(pipes[i][WRITE_P]);
        }
        for(i=0; i<c_pipes ;i++){
            int ret_pid=wait(NULL);
            printf("processo %d ha terminato\n",ret_pid);
            int j=0;
            int k=-1;
            for(j=0;j<c_pipes && k==-1;j++){
                if(ret_pid==map_pipes[j]){
                    k=j;
                }
            }
            printf("legge dal canale del figlio %d \n",ret_pid);
            for(j=0;j<256;j++){

            //forse da con

                char buff[sizeof(int)];
                read(pipes[k][READ_P],&buff,sizeof(int));
                //printf("%s\n",buff);
                caratteri[j]+=atoi(buff);
            }
            close(pipes[k][READ_P]);
        }
    } else{
        char arg_n[10];
        char arg_m[10];
        char arg_c[10];

        sprintf(arg_n,"%d%d",n,i);
        sprintf(arg_m,"%d",m);
        sprintf(arg_c,"%d",tmp_m);
        //printf("%s %s %s\n",arg_n,arg_m,arg_c);
        //printf("%s\n",paths[i-1]);
        close(pipes[c_pipes][READ_P]);
        dup2(pipes[c_pipes][WRITE_P],4);
        execlp("./q","q","-f",paths[i-1],"-n",arg_n,"-m",arg_m,"-c",arg_c,NULL);
        //system("echo errore");
        return -1;
    }


    printf("si stampa\n");
    int j=0;
    for(j=0;j<256;j++){
        printf("%d %d\n",j,caratteri[j]);
    }

    return 0;
}
