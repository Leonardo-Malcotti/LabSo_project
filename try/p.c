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

int main(int argc, char *argv[]) {
    int i;//indice per i for
    int n= 0;
    int m= 0;
    int ct = 0;
    //primi controlli sugli argomenti
    //i parametri dovrebbero essere almeno 7:
    //1 per il nome dell'eseguibile
    //4 per n,m e i loro valori
    //1 per -f
    //poi almeno 1 per inizializzare f

    //numero di files specificati
    char * paths[argc-6];
    int contr_arg[3] = {0,0,0};

    //controlli e lettura dei parametri
    for(i=1;i<argc;i++){

        //viene posto ad 1 se viene letto un parametro corretto
        int controllo = 0;

        //lettura parametro -n con dovuti controlli
        if(param_check(argv[i],ARG_N,contr_arg) == 0){
            //segnala che è stato letto un parametro valido
            controllo =1;
            if((n = str_to_int(argv[i+1])) == -1){
                //stampa errori
                printf("p: errore nel parametro n\n");
                exit(-1);
            }
            i++;
        }

        //lettura parametro -m con dovuti controlli
        if(param_check(argv[i],ARG_M,contr_arg) == 0){
            //segnala che è stato letto un parametro valido
            controllo =1;
            if((m = str_to_int(argv[i+1])) == -1){
                //stampa errori
                printf("p: errore nel parametro m\n");
                exit(-1);
            }
            i++;
        }

        //legge i valori del parametro -f
        if(param_check(argv[i],ARG_F,contr_arg) == 0){
            //segnala che è stato letto un parametro valido
            controllo =1;
            while(i+1 < argc && argv[i+1][0]!='-'){
                paths[ct] = argv[i+1];
                i++;
                ct++;
            }
        }

        if(controllo == 0){
            printf("parametri non validi\n");
            exit(-1);
        }
    }

    //se non sono stati specificati dei file allora è un processo con un gruppo vuoto
    //dunque termina direttamente il processo
    if(ct>0){
        //fa un fork per ogni file
        int pid= -1;
        int tmp_m=1;
        //tiene salvata una pipe per ogni sottoprocesso, utilizzando map_pipes per identificarle
        int pipes[(argc-6)*m][2];
        int map_pipes[(argc-6)*m];

        //valori dei caratteri suddivisi per file
        int val_per_file[argc-6][N_CARATTERI];
        int k=0;
        for(i=0;i<argc-6;i++){
            for(k=0;k<N_CARATTERI;k++){
                val_per_file[i][k]=0;
            }
        }

        //conta il numero di pipe presenti
        int c_pipes =0;

        for(i=0; i<argc-6 && pid!=0;i++){
            tmp_m=1;
            while(pid!=0 && tmp_m<=m){
                pipe(pipes[c_pipes]);
                pid = fork();
                if(pid<0){
                    printf("\n%s\n",strerror(errno));
                    exit(-1);
                }
                if(pid!=0){
                    map_pipes[c_pipes]=pid;
                    c_pipes++;
                    tmp_m++;
                }
            }
        }

        int caratteri[N_CARATTERI]={[0 ... N_CARATTERI-1]=0};
        if(pid!=0){

            for(i=0; i<c_pipes ;i++){
                close(pipes[i][WRITE_P]);
            }

            for(i=0; i<c_pipes ;i++){
                int ret_pid=wait(NULL);
                int j=0;
                int k=-1;

                for(j=0;j<c_pipes && k==-1;j++){
                    if(ret_pid==map_pipes[j]){
                        k=j;
                    }
                }
                int ln_lett;
                char buff[100];
                strcpy(buff,"");
                //legge l'id del file
                int contr= read_until_char(pipes[k][READ_P],'\n',buff,&ln_lett);
                if(contr<0){
                    printf("p: errore lettura id file dalla pipe\n");
                    //exit(-1);
                } else {
                    int id_f = str_to_int(buff);
                    if(id_f<0){
                        printf("p: errore conversione id file\n");
                        exit(-1);
                    }else{
                        for(j=0;j<N_CARATTERI;j++){
                            strcpy(buff,"");
                            contr = read_until_char(pipes[k][READ_P],'\n',buff,&ln_lett);
                            if(contr>=0){
                                int cc=str_to_int(buff);
                                if(cc<0){
                                    printf("p: errore conversione\n");
                                }else{
                                    caratteri[j]+=cc;
                                    val_per_file[id_f][j]+=cc;
                                }

                            } else {
                                printf("p: errore lettura valore da pipe\n");
                                printf("appartenente al file %s\n",paths[id_f]);
                                exit(-1);
                            }
                        }
                    }
                }
                close(pipes[k][READ_P]);
            }
        } else{
            char *arg_n=(char *)calloc(int_len(i),sizeof(char));
            char *arg_m=(char *)calloc(int_len(m),sizeof(char));
            char *arg_c=(char *)calloc(int_len(tmp_m),sizeof(char));
            //al parametro n passa il numero del gruppo e il numero di file (n i)
            sprintf(arg_n,"%d",(i-1));
            sprintf(arg_m,"%d",m);
            sprintf(arg_c,"%d",tmp_m);

            close(pipes[c_pipes][READ_P]);

            dup2(pipes[c_pipes][WRITE_P],PIPE_CHANNEL);
            execlp("./q","q","-f",paths[i-1],"-n",arg_n,"-m",arg_m,"-c",arg_c,NULL);
            return -1;
        }

        for(i=0;i<N_CARATTERI;i++){
            char *buff=(char *)calloc(int_len(caratteri[i])+1,sizeof(char));
            sprintf(buff,"%d\n",caratteri[i]);
            write(5,buff,strlen(buff));
            free(buff);
        }
        for(k=0;k<argc-6;k++){
            write(5,paths[k],strlen(paths[k]));
            write(5,"\n",1);
            for(i=0;i<N_CARATTERI;i++){
                char *buff=(char *)calloc(int_len(val_per_file[k][i])+1,sizeof(char));
                sprintf(buff,"%d\n",val_per_file[k][i]);
                write(5,buff,strlen(buff));
                free(buff);
            }
        }
        close(PIPE_CHANNEL);

    }
    return 0;
}
