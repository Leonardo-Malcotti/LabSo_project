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
#include "projectLib.h"

/*
Riceve in come parametri n, m e dei file divisi tra indirizzi diretti e cartelle
ha il compito di rilevare tutti i file specificati e suddividerli in n gruppi
creando poi n sottoprocessi p per gestire i file.
*/

int main(int argc, char *argv[]) {
    int i,j;//indice per i for
    int n=0;
    int m=0;
    //serve a contare i file specificati
    int ct=0;

    int contr_arg[3] = {0,0,0};

    //tiene traccia di dove inizia la lista di file in argv
    int pos_files=-1;

    for(i=1;i<argc;i++){
        //viene posto ad 1 se viene letto un parametro corretto
        int controllo = 0;

        //lettura parametro -n con dovuti controlli
        if(param_check(argv[i],ARG_N,contr_arg) == 0){
            //segnala che è stato letto un parametro valido
            controllo =1;
            if((n = str_to_int(argv[i+1])) == -1){
                //stampa errori
                printf("c: errore nel parametro n\n");
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
                printf("c: errore nel parametro m\n");
                exit(-1);
            }
            i++;
        }

        //legge i valori del parametro -f
        if(param_check(argv[i],ARG_F,contr_arg) == 0){

            //segnala che è stato letto un parametro valido
            controllo =1;
            pos_files=i;
            while(i+1 < argc && argv[i+1][0]!='-'){
                char *tmp_path = argv[i+1];
                int contr = is_dir(tmp_path);
                if(contr<0){
                    printf("\nerrore: %s non esiste o non è accessibile\n\n",tmp_path);
                    exit(-1);
                }
                if(contr==1){
                    int contr_dir=0;
                    int n_file = n_files_in_dir_subdir(tmp_path);
                    ct+=n_file;
                } else {
                    ct++;
                }
                i++;
            }
        }

        if(controllo == 0){
            printf("parametri non validi\n");
            exit(-1);
        }

    }




    if(ct>0){
        //una volta letti i parametri siamo sicuri che siano stati specificati correttamente
        //e ct contiene il numero di file passati tra cartelle e non
        //quindi adesso si possono salvare gli indirizzi
        char **files = (char **)malloc(ct*PATH_MAX*sizeof(char));
        int val_per_file[ct][N_CARATTERI];
        for(i=0;i<ct;i++){
            for(j=0;j<N_CARATTERI;j++){
                val_per_file[i][j]=0;
            }
        }


        int caratteri[N_CARATTERI]={[0 ... N_CARATTERI-1]=0};
        int k=pos_files;

        int p=0;
        while(k+1 < argc && argv[k+1][0]!='-'){
            char *tmp_path = argv[k+1];
            if(is_dir(tmp_path)==1){
                //se è una directory allora cerca tutti i file nelle sottodirectory se ci sono
                files_in_dir_subdir(tmp_path,&p,files);
            } else {
                files[p]=tmp_path;
                p++;
            }
            k++;
        }


        int pid= -1;

        //tiene salvata una pipe per ogni sottoprocesso, utilizzando map_pipes per identificarle
        int pipes[n][2];
        int map_pipes[n];

        //conta il numero di pipe presenti
        int c_pipes=0;

        //numero di file per ogni gruppo
        int nf_group=(ct%n!=0)? (ct/n)+1 : ct/n;

        //usati per segnare da dove iniziare a leggere da files a dove finire
        int first_file=0;
        int last_file=nf_group;


        for(i=0;i<n && pid!=0;i++){
            pipe(pipes[c_pipes]);
            pid=fork();
            if(pid<0){
                printf("\n%s\n",strerror(errno));
                exit(-1);
            }
            if(pid!=0){
                map_pipes[c_pipes]=pid;
                c_pipes++;

                first_file=last_file;
                //c'è da considerare che potrebbe esserci un resto
                last_file=(last_file+nf_group>ct)?ct : last_file+nf_group;
            }
        }

        //il processo padre attende che tutti i figli terminino
        //e legge i risultati dalle pipe
        if(pid!=0){
            for(i=0; i<c_pipes ;i++){
                close(pipes[i][WRITE_P]);
            }

            for(i=0; i<c_pipes ;i++){
                int ret_pid=wait(NULL);
                int k=-1;
                for(j=0;j<c_pipes && k==-1;j++){
                    if(ret_pid==map_pipes[j]){
                        k=j;
                    }
                }
                int contr=0;
                int ln_lett;
                char buff[PATH_MAX];

                for(j=0;j<N_CARATTERI;j++){
                    strcpy(buff,"");
                    contr = read_until_char(pipes[k][READ_P],'\n',buff,&ln_lett);
                    int cc = str_to_int(buff);
                    if(cc<0){
                        printf("c: errore nella conversione di un valore letto da pipe\n");
                    } else {
                        caratteri[j]+=cc;
                    }
                }
                //legge il resto della pipe per i dati dei singoli file
                while(contr!=-1){
                    strcpy(buff,"");
                    contr = read_until_char(pipes[k][READ_P],'\n',buff,&ln_lett);
                    if(contr!=-1){
                        int id_file=-1;
                        for(j=0;j<ct && id_file==-1;j++){
                            if(strcmp(buff,files[j])==0){
                                id_file=j;
                            }
                        }

                        if(id_file!=-1){
                            for(j=0;j<N_CARATTERI;j++){
                                strcpy(buff,"");
                                contr = read_until_char(pipes[k][READ_P],'\n',buff,&ln_lett);

                                int cc = str_to_int(buff);
                                if(cc<0){
                                    printf("c: errore nella conversione di un file letto da pipe\n");
                                    printf("sezione file %s\n",files[id_file]);
                                } else {
                                    val_per_file[id_file][j]+=cc;
                                }
                            }
                        }
                    }
                }

                close(pipes[k][READ_P]);
            }
        }
        //prepara la lista di argomenti e avvia p
        else{

            char *arg_n=(char *)calloc(int_len(n),sizeof(char));
            char *arg_m=(char *)calloc(int_len(m),sizeof(char));

            //i contiene il numero del gruppo di file a questo punto
            sprintf(arg_n,"%d",i);
            sprintf(arg_m,"%d",m);

            //array contenente gli argomenti del sottoprocesso
            int len_argv = last_file-first_file+7;

            char **argv_p = malloc(len_argv * sizeof(argv_p[0]));
            argv_p[0]="p";
            argv_p[1]="-n";
            argv_p[2]=arg_n;
            argv_p[3]="-m";
            argv_p[4]=arg_m;
            argv_p[5]="-f";

            j=6;

            for(i=first_file;i<last_file;i++){
                argv_p[j]=files[i];
                j++;
            }

            argv_p[j]=(char *)NULL;

            close(pipes[c_pipes][READ_P]);
            dup2(pipes[c_pipes][WRITE_P],5);

            execvp("./p",(char *const*)argv_p);

            printf("%s\n",strerror(errno));
            return -1;
        }

        int ctr = open("report.txt",O_WRONLY|O_CREAT,S_IRWXU);
        int stat[8]={[0 ... 7]=0};
        char parole[8][20]={"TOTALE","MAIUSCOLE","MINUSCOLE","CONSONANTI","VOCALI","PUNTEGGIATURA","SPECIALI","NUMERI"};

        //stampo come primo valore la quantità di file analizzati
        char * nn = (char *)calloc(int_len(ct)+1,sizeof(char));
        sprintf(nn,"%d\n",ct);
        write(ctr,nn,strlen(nn));
        free(nn);

        //calcola le ricorrenze per tipo di carattere
        for(j=0;j<N_CARATTERI;j++){
            char_type(stat,caratteri,j);
        }

        for(j=0;j<N_CARATTERI;j++){
            char *line=(char *)calloc(int_len(j)+int_len(caratteri[j])+13,sizeof(char));
            //double perc = (stat[TOTALE]==0)? 0.0 : ((double)caratteri[j]/stat[TOTALE])*100.0;
            sprintf(line,"%d %d\n",j,caratteri[j]);
            write(ctr,line,strlen(line));
            free(line);
        }
        for(i=0;i<8;i++){
            char *parola=(char*)calloc(20+int_len(stat[i])+13,sizeof(char));
            //double perc = (stat[TOTALE]==0)? 0.0 : ((double)stat[i]/stat[TOTALE])*100.0;
            sprintf(parola,"%s %d\n",parole[i],stat[i]);
            write(ctr,parola,strlen(parola));
            free(parola);
        }
        //azzera le statistiche
        for(i=0;i<8;i++){
            stat[i]=0;
        }

        for(i=0;i<ct;i++){
            //calcola le ricorrenze per tipo di carattere

            for(j=0;j<N_CARATTERI;j++){
                char_type(stat,val_per_file[i],j);
            }

            write(ctr,files[i],strlen(files[i]));
            write(ctr,"\n",strlen("\n"));

            for(j=0;j<N_CARATTERI;j++){
                char *line=(char *)calloc(int_len(j)+int_len(val_per_file[i][j])+13,sizeof(char));
                //double perc = (stat[TOTALE]==0)? 0.0 : ((double)val_per_file[i][j]/stat[TOTALE])*100.0;
                sprintf(line,"%d %d\n",j,val_per_file[i][j]);
                write(ctr,line,strlen(line));
                free(line);
            }
            for(j=0;j<8;j++){
                char *parola=(char*)calloc(20+int_len(stat[j])+13,sizeof(char));
                //double perc = (stat[TOTALE]==0)? 0 : ((double)stat[j]/stat[TOTALE])*100.0;
                sprintf(parola,"%s %d\n",parole[j],stat[j]);
                write(ctr,parola,strlen(parola));
                free(parola);
            }

            //azzera le statistiche
            for(j=0;j<8;j++){
                stat[j]=0;
            }
        }
        close(ctr);
        free(files);

    }

    return 0;
}
