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
#include <math.h>
#include "projectLib.h"

//Array universale per i parametri, da accedere utilizzando le costanti ARG_*
char *arr_param[] = {"-n","-m","-f","-c"};


void print_p_help(){
    printf("\n");
    printf("-f  indica il percorso del file di input.\n");
    printf("-n  indica il numero del gruppo di file.\n");
    printf("    se non specificato n sarà 0 di default.\n");
    printf("-m  indica il numero di parti in cui il file verrà suddiviso.\n");
    printf("    se non specificato sarà 0, quindi verrà analizzato tutto il file.\n");
    printf("\n");
}


void print_q_help(){
    printf("\n");
    printf("parametri\n\n");
    printf("-f  indica il percorso del file di input.\n");
    printf("-n  indica il numero del gruppo di file.\n");
    printf("    se non specificato n sarà 0 di default.\n");
    printf("-m  indica il numero di parti in cui il file è da suddividere.\n");
    printf("    deve essere un numero positivo diverso da 0, se non specificato darà errore\n");
    printf("-c  indica quale parte analizzare, se a 0 analizza tutto il file.\n");
    printf("    se non specificato sarà 0.\n");
    printf("\n");
}


void print_r_help(){
    printf("\n");
    printf("parametri\n\n");
    printf("-f  indica il percorso del file di input.\n");
    printf("\n");
}


void print_m_help(){
    printf("\n");
    printf("  comandi:\n");
    printf("\n  --generali:\n");
    printf("    h, help, man : stampa questo messaggio d'aiuto.\n");
    printf("    q, quit, exit : termina l'esecuzione del programma.\n");
    printf("\n  --parametri di default:\n");
    printf("    n : modifica il valore di default di n.\n");
    printf("    m : modifica il valore di default di m.\n");
    printf("    def, default : visualizza i valori di default attuali di n e m.\n");
    printf("\n    per modificare n e m è possibile usare la forma contratta\n");
    printf("    specificando il valore desiderato sulla stessa riga del comando suddiviso da uno spazio\n");
    printf("\n");
}


int param_check(char *arg,int arg_type,int arr_check[]){
    if(strcmp(arg,arr_param[arg_type]) == 0){
        if(arr_check[arg_type]!= 0){
            printf("hai usato %s troppe volte\n",arr_param[arg_type]);
            //print_p_help();
            return -1;
        } else {
            arr_check[arg_type] = 1;
            return 0;
        }
    }
    return -1;
}


int str_to_int(char * arg){
    char *err;
    //tenta di convertirlo in long
    long int tmp = strtol(arg,&err,10);
    //se err viene inizializzato allora la stringa passata non era interamente di numeri
    if(*err){
        printf("errore nella conversione della stringa\n");
        return -1;
    }
    if(tmp<0){
        printf("numero risultante negativo\n");
        return -1;
    }
    return (int)tmp;
}


int open_file(char * arg,int *len){
    //la struttura stat contiene varie informazioni riguardanti i file
    struct stat buff;
    int tmp = open(arg,O_RDWR);
    //genera le statistiche del file
    stat(arg,&buff);
    if(tmp < 0){
        //
        //inserire controllo errori
        //
        printf("errore nel file passato\n");
        return -1;
    }
    //legge la lunghezza dello stream dalle statistiche
    *len = (int)buff.st_size;
    return tmp;
}


int is_dir(char * arg){
    struct stat buff;
    int contr = stat(arg,&buff);
    if(contr<0){
        //
        //controlli
        //
        return -1;
    } else {
        return S_ISDIR(buff.st_mode);
    }
}


int pipe_system_command(int pip[2],char *command){
    int contr = 0;
    contr=pipe(pip);
    if(contr<0){
        //
        //errori
        //
        return contr;
    }
    int std = dup(STDOUT_FILENO);
    if(std<0){
        //
        //errori
        //
        return std;
    }
    contr=dup2(pip[WRITE_P],STDOUT_FILENO);
    if(contr<0){
        //
        //errori
        //
        return contr;
    }
    contr=system(command);
    if(contr<0){
        //
        //errori
        //
        return contr;
    }
    fflush(NULL);
    contr=dup2(std,STDOUT_FILENO);
    if(contr<0){
        //
        //errori
        //
        return contr;
    }
    return 0;
}


int read_until_char(int des,char str,char *buf, int *len){
    int rd=0;
    int c=0;
    do{
        //buffer di supporto
        char buf2[3];
        strcpy(buf2,"");
        rd=read(des,buf2,1);
        if(*buf2==str){
            *len=c;
            return 0;
        } else {
            strcat(buf,buf2);
            c++;
        }
    }while(rd>0);
    return -1;
}


int n_files_in_dir(char * path){
    //
    //fare controllo sulla dir?
    //
    int pip[2];
    //creazione del comando per contare il numero di file in una cartella
    char command[strlen(path)+11];//ls path | wc -l
    strcpy(command,"ls ");
    strcat(command,path);
    strcat(command," | wc -l");
    fflush(NULL);

    //avvia il comando facendo in modo che il risultato sia salvato su una pipe
    int contr = pipe_system_command(pip,command);
    if(contr<0){
        return -1;
    }
    close(pip[WRITE_P]);

    //salva in una stringa (buf) il contenuto della prima riga della pipe
    char buf[COSTANTE_LIMITE_TEMPORANEA];
    strcpy(buf,"");
    int len_buf;
    contr = read_until_char(pip[READ_P],'\n',buf,&len_buf);
    close(pip[READ_P]);

    //il problema maggiore di wc è che stampa il risultato con dei tab in testa
    //quindi una conversione diretta di buf darebbe degli errori, dato che non
    //è composto solo da char di numeri

    int ret=-1;
    int i=0;

    //eseguo la conversione spostando l'inizio della stringa di una posizione finchè non ha successo
    for(i=0;i<len_buf;i++){
        ret = str_to_int(&buf[i]);
        if(ret!=-1){
            i=len_buf;
        }
    }

    return (int)ret;
}


int n_files_in_dir_subdir(char * path){
    int num = n_files_in_dir(path);
    int tmp = num;
    int i=0;
    //matrice che conterrà i percorsi dei file
    char **tmp_f=(char **)malloc(num*PATH_MAX*sizeof(char));
    files_in_dir(path,tmp_f);

    //controlla per ogni elemento di tmp_f se è una cartella
    //se lo è avvia la ricorsione e toglie 1 dal conteggio
    for(i=0;i<tmp;i++){
        if(is_dir(tmp_f[i])==1){
            num--;
            num+=n_files_in_dir_subdir(tmp_f[i]);
        }
    }
    return num;
}


void files_in_dir(char * path,char ** ret){
    //fa in modo che venga scritto nella pipe la lista dei file
    int tmp_pipe[2];
    char command[strlen(path)+3];
    strcpy(command,"ls ");
    strcat(command,path);
    pipe_system_command(tmp_pipe,command);
    close(tmp_pipe[WRITE_P]);

    //legge il contenuto della pipe
    int j=0;
    int p=0;
    while(j==0){
        int len_str;
        char buf[NAME_MAX];
        strcpy(buf,"");
        j =read_until_char(tmp_pipe[READ_P],'\n',buf,&len_str);
        if(j==0){
            //stringa di supporto
            char * str = (char *)malloc(sizeof(path)+sizeof(buf)+1);
            strcpy(str,"");
            strcat(str,path);
            strcat(str,"/");
            strcat(str,buf);
            ret[p]=str;
            p++;
        }
    }
    close(tmp_pipe[READ_P]);
}


void files_in_dir_subdir(char * path, int * p, char **buf){
    int i=0;
    //salva temporaneamente il numero di file/cartelle in path
    int tmp_nf_dir = n_files_in_dir(path);

    //salva temporaneamente il contenuto di path
    char ** tmp_f_dir = (char **)malloc(tmp_nf_dir*PATH_MAX*sizeof(char));
    files_in_dir(path,tmp_f_dir);

    //controlla ogni elemento di tmp_f_dir
    //se è una cartella avvia la ricorsione
    //se non lo è lo aggiunge a buf e aumenta p
    for(i=0;i<tmp_nf_dir;i++){
        if(is_dir(tmp_f_dir[i])==1){
            files_in_dir_subdir(tmp_f_dir[i],p,buf);
        } else {
            buf[*p]=tmp_f_dir[i];
            (*p)++;
        }
    }
}


char * read_input(){
    char *s=(char *)malloc(BUFFER_IN_LEN*sizeof(char));
    s = fgets(s,BUFFER_IN_LEN,stdin);
    int i=0;
    for(i=0;i<strlen(s);i++){
        if(s[i]=='\n'|| s[i]==EOF){
            s[i]='\0';
        }
    }
    return s;
}
