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
    int i=0;
    int def_m=1;
    int def_n=1;
    int exit =-1;
    while(exit==-1){
        int cmd_valido=0;
        printf("~ ");

        char *s=read_input();
        int len_s;

        //comandi d'uscita dal processo
        if(strcmp(s,"exit")==0 || strcmp(s,"q")==0 || strcmp(s,"quit")==0){
            exit=0;
            cmd_valido=1;
        }

        //comandi per la stampa del messaggio d'aiuto
        if(strcmp(s,"help")==0 || strcmp(s,"h")==0 || strcmp(s,"man")==0){
            print_m_help();
            cmd_valido=1;
        }

        //comando verboso per la modifica di n
        if(strcmp(s,"n")==0){
            printf("\ninserire il nuovo valore di default di n:\n\n");
            do{
                printf("> ");
                char *tmp=read_input();
                def_n=str_to_int(tmp);
                if(def_n<=0){
                    printf("\n  valore inserito non valido\n");
                    printf("  assicurarsi di inserire un numero positivo\n\n");
                }
            }while(def_n<=0);
            cmd_valido=1;
        }

        //comando verboso per la modifica di m
        if(strcmp(s,"m")==0){
            printf("\ninserire il nuovo valore di default di m:\n\n");
            do{
                printf("> ");
                char *tmp=read_input();
                def_m=str_to_int(tmp);
                if(def_m<=0){
                    printf("\n  valore inserito non valido\n");
                    printf("  assicurarsi di inserire un numero positivo\n\n");
                }
            }while(def_m<=0);
            cmd_valido=1;
        }

        //comandi contratti per la modifica di n o m
        if(strlen(s)>=3 && cmd_valido==0){
            if(s[0]=='n' && s[1]==' '){
                int tmp = str_to_int(&s[2]);
                if(tmp<=0){
                    printf("\n  valore specificato per n non valido\n\n");
                } else {
                    def_n=tmp;
                }
                cmd_valido=1;
            }

            if(s[0]=='m' && s[1]==' '){
                int tmp = str_to_int(&s[2]);
                if(tmp<=0){
                    printf("\n  valore specificato per m non valido\n\n");
                } else {
                    def_m=tmp;
                }
                cmd_valido=1;
            }
        }

        //comando per la stampa dei valori di default di n e m
        if(strcmp(s,"def")==0 || strcmp(s,"default")==0){
            printf("\n  valori di default:\n");
            printf("\n  n = %d",def_n);
            printf("\n  m = %d",def_m);
            printf("\n\n");
            cmd_valido=1;
        }


        //nel caso la stringa immessa non corrisponda a nessuna della precedenti
        //si stampa un avvertimento
        if(cmd_valido==0){
            printf("\n  comando inserito non riconosciuto.\n");
            printf("  scrivere help per visualizzare la guida.\n\n");
        }
    }
    return 0;
}
