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

    int contr_arg[3] = {0,0,0};

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

            int ct = 0;
            while(i+1 < argc && argv[i+1][0]!='-'){
                //char tmp[strlen(argv[i+1])];
                char *tmp_path = argv[i+1];
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
                        j =read_until_n(tmp_pipe[READ_P],buf,&len_str);

                        if(j==0){printf("%s\n",buf);}

                    }
                    close(tmp_pipe[READ_P]);
                }
                i++;
                ct++;
            }
        }

        if(controllo == 0){
            printf("parametri non validi\n");
            exit(-1);
        }

    }

    return 0;
}
