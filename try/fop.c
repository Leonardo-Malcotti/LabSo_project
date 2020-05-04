#include <stdio.h>
#include <errno.h>
#include <string.h>

//EISDR : valore di errno quando fopen cerca di aprire una cartella in w o r+
//

int main(int argn, char *argv[]){

    if(argn != 2){
        printf("troppi argomenti\n");
        return 0;
    }

    char path[strlen(argv[1])+1];
    strcpy(path,argv[1]);

    FILE *f = fopen(path,"r+");
    if(f == NULL){
        //controlla se l'errore è dovuto al passaggio di una cartella
        if(errno == EISDIR){
            printf("yey\n");
            //stringa per contenere il comando
            char command[10+strlen(path)];
            //inserisce il comando per cambiare cartella
            strcpy(command,"cd ");
            //specifica il path
            strcat(command,path);
            //stampa il contenuto
            strcat(command," && ls");
            printf("%s\n",command);
            system(command);
        }
        return 0;
    }

    fclose(f);
    return 0;
}
