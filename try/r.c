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

#define F 0
#define G 1
#define C 2
#define V 3
#define N 4
#define P 5

int main(int argc, char *argv[]) {

    int i,x,j;//indice per i for

    char buf[PATH_MAX];
    strcpy(buf,"");
    int contr,len;
    int tmp;

    int contr_arg[6]={0,0,0,0,0,0};

    //apertura file
    int finput,len_file;
    if((finput=open_file("report.txt",&len_file))==-1){
        printf("r: file di report non trovato\n");
        exit(-1);
    }

    int pos_files;
    int ct;

    for(i=1;i<argc;i++){
        //viene posto ad 1 se viene letto un parametro corretto
        int controllo = 0;

        if(strcmp(argv[i],"-f") == 0){
            //segnala che è stato letto un parametro valido
            if(contr_arg[F]==1){
                printf("r: errore nei parametri\n");
                exit(-1);
            }
            contr_arg[F]=1;

            controllo =1;
            pos_files=i;
            while(i+1 < argc && argv[i+1][0]!='-'){
                char *tmp_path = argv[i+1];
                int contr = is_dir(tmp_path);
                if(contr==1){
                    printf("valore non valido, %s è una cartella\n",tmp_path);
                    exit(-1);
                } else {
                    ct++;
                }
                i++;
            }
        }

        if(strcmp(argv[i],"-g")==0){
            if(contr_arg[G]==1){
                printf("r: errore nei parametri\n");
                exit(-1);
            }
            contr_arg[G]=1;

            controllo =1;
        }

        if(strcmp(argv[i],"-c")==0){
            if(contr_arg[C]==1){
                printf("r: errore nei parametri\n");
                exit(-1);
            }
            contr_arg[C]=1;

            controllo =1;
        }

        if(strcmp(argv[i],"-v")==0){
            if(contr_arg[V]==1){
                printf("r: errore nei parametri\n");
                exit(-1);
            }
            contr_arg[V]=1;

            controllo =1;
        }

        if(strcmp(argv[i],"-n")==0){
            if(contr_arg[N]==1){
                printf("r: errore nei parametri\n");
                exit(-1);
            }
            contr_arg[N]=1;

            controllo =1;
        }

        if(strcmp(argv[i],"-p")==0){
            if(contr_arg[P]==1){
                printf("r: errore nei parametri\n");
                exit(-1);
            }
            contr_arg[P]=1;

            controllo =1;
        }

        if(controllo == 0){
            printf("parametri non validi\n");
            exit(-1);
        }
    }

    char gruppi[8][20]={"TOTALE","MAIUSCOLE","MINUSCOLE","CONSONANTI","VOCALI","PUNTEGGIATURA","SPECIALI","NUMERI"};
    char caratteri1 [33][4] = {"nul","soh","stx","etx","eot","enq","ack","bel","bs","ht","nl","vt","ff","cr","so","si","dle","dc1","dc2","dc3","dc4","nak","syn","etb","can","em","sub","esc","fs","gs","rs","us","sp"};

    strcpy(buf,"");
    contr=read_until_char(finput,'\n',buf,&len);
    int n_file_analizzati=str_to_int(buf);
    if(n_file_analizzati<1){
        printf("r: errore nella lettura del primo parametro di report.txt\n");
        exit(-1);
    }
    int val_gen[N_CARATTERI+8];
    char **files=(char **)calloc(ct*PATH_MAX,sizeof(char));
    int val_per_file[ct][N_CARATTERI+8];

    int k=pos_files;
    i=0;
    while(k+1 < argc && argv[k+1][0]!='-'){
        files[i]=strdup(argv[k+1]);
        i++;
        k++;
    }

    //
    //lettura file di report e creazione matrici
    //
    for(i=0;i<N_CARATTERI;i++){
        int car;
        int val;
        strcpy(buf,"");
        contr = read_until_char(finput,' ',buf,&len);
        if(contr<0){
            //errore nella lettura
            exit(-1);
        }
        car=str_to_int(buf);
        if(car>=0){
            strcpy(buf,"");
            contr = read_until_char(finput,'\n',buf,&len);
            if(contr<0){
                //errore nella lettura
                exit(-1);
            }
            val=str_to_int(buf);
            if(val>=0){
                val_gen[car]=val;
            }
        }
    }

    for(i=N_CARATTERI;i<N_CARATTERI+8;i++) {

        int val;
        //lettura a vuoto
        strcpy(buf,"");
        contr = read_until_char(finput,' ',buf,&len);
        if(contr<0){
            //errore nella lettura
            exit(-1);
        }
        //leggo il valore effettivo del gruppo
        strcpy(buf,"");
        contr = read_until_char(finput,'\n',buf,&len);
        if(contr<0){
            //errore nella lettura
            exit(-1);
        }
        val=str_to_int(buf);
        if(val>=0){
            val_gen[i]=val;
        }
    }

    for(j=0;j<n_file_analizzati;j++){
        strcpy(buf,"");
        contr = read_until_char(finput,'\n',buf,&len);

        int id_file=-1;
        for(x=0; x<ct && id_file==-1 ;x++){
            if(strcmp(files[x],buf)==0){
                id_file=x;
            }
        }
        //se non è tra i file di cui stampare il report salta la sezione di file
        if(id_file==-1){
            for(x=0;x<N_CARATTERI+8;x++){
                strcpy(buf,"");
                contr = read_until_char(finput,'\n',buf,&len);
            }
        }else{
            for(i=0;i<N_CARATTERI;i++){
                int car;
                int val;
                strcpy(buf,"");
                contr = read_until_char(finput,' ',buf,&len);
                if(contr<0){
                    //errore nella lettura
                    exit(-1);
                }
                car=str_to_int(buf);
                if(car>=0){
                    strcpy(buf,"");
                    contr = read_until_char(finput,'\n',buf,&len);//fino a \n dopo
                    if(contr<0){
                        //errore nella lettura
                        exit(-1);
                    }
                    val=str_to_int(buf);
                    if(val>=0){
                        val_per_file[id_file][car]=val;
                    }
                }
            }
            for(i=N_CARATTERI;i<N_CARATTERI+8;i++){
                int val;
                //lettura a vuoto
                strcpy(buf,"");
                contr = read_until_char(finput,' ',buf,&len);
                if(contr<0){
                    //errore nella lettura
                    exit(-1);
                }
                //leggo il valore effettivo del gruppo
                strcpy(buf,"");
                contr = read_until_char(finput,'\n',buf,&len);
                if(contr<0){
                    //errore nella lettura
                    exit(-1);
                }
                val=str_to_int(buf);
                if(val>=0){
                    val_per_file[id_file][i]=val;
                }
            }
        }
    }

    //
    //stampa tabelle generali
    //

    if(contr_arg[G]==0){
        printf("REPORT\n\nGeneral statistics:\n");
        int cc=0;
        if(contr_arg[C]==1){
            cc=1;
            printf("consonanti\n");
            r_stampa_consonanti(val_gen);
            printf("\n");
        }
        if(contr_arg[V]==1){
            cc=1;
            printf("vocali\n");
            r_stampa_vocali(val_gen);
            printf("\n");
        }
        if(contr_arg[N]==1){
            cc=1;
            printf("numeri\n");
            r_stampa_numeri(val_gen);
            printf("\n");
        }
        if(contr_arg[P]==1){
            cc=1;
            printf("punteggiatura\n");
            r_stampa_punteggiatura(val_gen);
            printf("\n");
        }
        if(cc==0){
            printf("\033[0;31m");
            for(x=0;x<8;x++){
                printf("| char ");
                printf("  freq ");
                printf("     %% |");
            }
            printf("\033[0m");
            printf("\n");
            r_stampa_tutto(val_gen);
        }
    }

    //
    //stampa tabelle file singoli
    //
    if(contr_arg[F]==1){
        for(x=0;x<ct;x++){
            printf("\033[0;31m");
            printf("%s\n",files[x]);
            printf("\033[0m");
            int cc=0;
            if(contr_arg[C]==1){
                cc=1;
                printf("consonanti\n");
                r_stampa_consonanti(val_per_file[x]);
                printf("\n");
            }
            if(contr_arg[V]==1){
                cc=1;
                printf("vocali\n");
                r_stampa_vocali(val_per_file[x]);
                printf("\n");
            }
            if(contr_arg[N]==1){
                cc=1;
                printf("numeri\n");
                r_stampa_numeri(val_per_file[x]);
                printf("\n");
            }
            if(contr_arg[P]==1){
                cc=1;
                printf("punteggiatura\n");
                r_stampa_punteggiatura(val_per_file[x]);
                printf("\n");
            }
            if(cc==0){
                r_stampa_tutto(val_per_file[x]);
            }

        }
    }

    close(finput);
    return 0;
}
