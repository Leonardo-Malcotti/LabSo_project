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

        //comando verboso per l'avvio di c
        if(strcmp(s,"c")==0){
            int pip_f[2];
            int cont=0;
            pipe(pip_f);
            printf("\n  elencare i file da analizzare\n");
            printf("  digitare q per terminare\n\n");
            char *in=(char*)malloc(PATH_MAX*sizeof(char));
            strcpy(in,"not q");
            while(strcmp(in,"q")!=0){
                printf("> ");
                in = read_input();
                if(strcmp(in,"q")!=0){
                    struct stat buff;
                    int contr = stat(in,&buff);
                    if(contr<0){
                        printf("\n  il file o percorso specificato non esiste o non è accessibile\n\n");
                    } else {
                        write(pip_f[WRITE_P],in,sizeof(in));
                        write(pip_f[WRITE_P],"\n",1);
                        cont++;
                    }
                }
            }
            close(pip_f[WRITE_P]);

            if(cont!=0){

                char arg_n[30];
                char arg_m[30];

                //i contiene il numero del gruppo di file a questo punto
                sprintf(arg_n,"%d",def_n);
                sprintf(arg_m,"%d",def_m);

                char ** argv_c = malloc((cont+7)*sizeof(argv_c[0]));
                argv_c[0]="c";
                argv_c[1]="-n";
                argv_c[2]=arg_n;
                argv_c[3]="-m";
                argv_c[4]=arg_m;
                argv_c[5]="-f";

                for(i=6;i<cont+7;i++){
                    int len=0;
                    char * str_tmp = (char *)malloc(PATH_MAX*sizeof(char));
                    int contr = read_until_char(pip_f[READ_P],'\n',str_tmp,&len);

                    argv_c[i]=str_tmp;
                }
                close(pip_f[READ_P]);
                argv_c[cont+6]=(char *)NULL;
                int id = fork();

                if(id==0){
                    execvp("./c",(char *const*)argv_c);
                    return -1;
                } else {
                    printf("\n  avvio del conteggio\n\n");
                }
            } else {
                close(pip_f[READ_P]);
                printf("\n  non sono stati specificati file o directory valide\n");
                printf("  processo annullato\n\n");
            }
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
