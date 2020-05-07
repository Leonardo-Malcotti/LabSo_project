#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

/*
o si trova il modo di rendere -m e -n obbligatori
o si trova il modo di non dover specificare la lunghezza di paths
*/

void print_help(){
    printf("\n");
    printf("-f  indica il percorso del file di input.\n");
    printf("-n  indica il numero del gruppo di file.\n");
    printf("    se non specificato n sarà 0 di default.\n");
    printf("-m  indica il numero di parti in cui il file verrà suddiviso.\n");
    printf("    se non specificato sarà 0, quindi verrà analizzato tutto il file.\n");
    printf("\n");
}


#define ARG_N 0
#define ARG_M 1
#define ARG_F 2

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
        if(strcmp(argv[i],"-n") == 0){
            //controlla che non sia stato usato più volte
            if(contr_arg[ARG_N]!= 0){
                printf("hai usato -n troppe volte\n");
                print_help();
                return 0;
            } else {
                contr_arg[ARG_N] = 1;
            }

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
        if(strcmp(argv[i],"-m") == 0){

            //controlla che non sia stato usato più volte
            if(contr_arg[ARG_M]!= 0){
                printf("hai usato -m troppe volte\n");
                print_help();
                return 0;
            } else {
                contr_arg[ARG_M] = 1;
            }

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
        if(strcmp(argv[i],"-f") == 0){

            //controlla che non sia stato usato più volte
            if(contr_arg[ARG_F]!= 0){
                printf("hai usato -f troppe volte\n");
                print_help();
                return 0;
            } else {
                contr_arg[ARG_F] = 1;
            }

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
    for(i=0; i<argc-6 && pid!=0;i++){
        tmp_m=1;
        //printf("%s\n",paths[i]);
        while(pid!=0 && tmp_m<=m){
            pid = fork();
            tmp_m++;
        }
    }
    //printf("%d %d %d\n",n,i,tmp_m);

    if(pid!=0){
        wait(NULL);
    } else{
        char arg_n[10];
        char arg_m[10];
        char arg_c[10];

        sprintf(arg_n,"%d%d",n,i);
        sprintf(arg_m,"%d",m);
        sprintf(arg_c,"%d",tmp_m);
        printf("%s %s %s\n",arg_n,arg_m,arg_c);
        printf("%s\n",paths[i-1]);
        execlp("./q","q","-f",paths[i-1],"-n",arg_n,"-m",arg_m,"-c",arg_c,NULL);
    }
    return 0;
}
