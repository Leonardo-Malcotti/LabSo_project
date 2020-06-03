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
    int i;//indice per i for
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
                if(is_dir(tmp_path)==1){
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


    //una volta letti i parametri siamo sicuri che siano stati specificati correttamente
    //e ct contiene il numero di file passati tra cartelle e non
    //quindi adesso si possono salvare gli indirizzi
    char **files = (char **)malloc(ct*PATH_MAX*sizeof(char));

    //[ct][PATH_MAX];
    if(ct>0){
        int caratteri[256]={[0 ... 255]=0};
        int k=pos_files;

        int p=0;
        while(k+1 < argc && argv[k+1][0]!='-'){
            char *tmp_path = argv[k+1];
            if(is_dir(tmp_path)==1){
                //se è una directory allora cerca tutti i file nelle sottodirectory se ci sono
                files_in_dir_subdir(tmp_path,&p,files);
            } else {
                strcpy(files[p],tmp_path);
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
                //printf("è finito %d\n",ret_pid);
                int j=0;
                int k=-1;
                for(j=0;j<c_pipes && k==-1;j++){
                    if(ret_pid==map_pipes[j]){
                        k=j;
                    }
                }
                for(j=0;j<256;j++){
                    int ln_lett;
                    char buff[sizeof(int)];
                    strcpy(buff,"");
                    int contr = read_until_char(pipes[k][READ_P],'\n',buff,&ln_lett);
                    //printf("%s\n",buff);
                    caratteri[j]+=str_to_int(buff);
                }
                close(pipes[k][READ_P]);
            }
        }
        //prepara la lista di argomenti e avvia p
        else{

            char arg_n[30];
            char arg_m[30];

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

            int j=6;

            for(i=first_file;i<last_file;i++){
                argv_p[j]=files[i];
                j++;
            }

            argv_p[j]=(char *)NULL;

            for(i=0;i<j;i++){
                //printf("%s ",argv_p[i]);
            }

            //char *const* arg_ref=(char *const*)argv_p;

            //arg_ref=argv_p;

            close(pipes[c_pipes][READ_P]);
            dup2(pipes[c_pipes][WRITE_P],5);

            execvp("./p",(char *const*)argv_p);

            printf("%s\n",strerror(errno));
            return -1;
        }
        int j=0;

        for(j=0;j<256;j++){
            printf("%d %d\n",j,caratteri[j]);
        }


    }

    return 0;
}
