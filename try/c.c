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

int main(int argc, char *argv[]) {
    int i;//indice per i for
    int n=0;
    int m=0;
    int ct=0;
    int contr_arg[3] = {0,0,0};
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
                    int n_file = files_in_dir(tmp_path);
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


    char *files[ct];
    if(ct>0){
        int k=pos_files;

        int p=0;
        while(k+1 < argc && argv[k+1][0]!='-'){
            char *tmp_path = argv[k+1];

            if(is_dir(tmp_path)==1){
                int tmp_pipe[2];
                char command[strlen(tmp_path)+3];
                strcpy(command,"ls ");
                strcat(command,tmp_path);

                pipe_system_command(tmp_pipe,command);
                close(tmp_pipe[WRITE_P]);

                //finchè j è a 0 allora la funzione ha letto qualcosa
                //quando sarà -1 allora il file è finito
                int j=0;
                while(j==0){
                    int len_str;
                    char buf[NAME_MAX];
                    strcpy(buf,"");
                    j =read_until_char(tmp_pipe[READ_P],'\n',buf,&len_str);

                    if(j==0){
                        //
                        //PROBLEMA CON LE STRINGHE
                        //
                        strcpy(files[p],"");
                        strcat(files[p],tmp_path);
                        printf("%s\n",tmp_path);
                        strcat(files[p],"/");
                        strcat(files[p],buf);
                        printf("%s\n",buf);
                        p++;
                    }

                }
                close(tmp_pipe[READ_P]);
            } else {
                files[p]=tmp_path;
                //strcpy(files[p],tmp_path);
                p++;

            }
            k++;
        }

        for(i=0;i<ct;i++){
            printf("%s\n",files[i]);
        }

    }

    return 0;
}
