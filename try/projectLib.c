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
#include "projectLib.h"

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
    printf("    se non specificato sarà 0, quindi verrà analizzato tutto il file.\n");
    printf("-c  indica quale parte analizzare, se a 0 analizza tutto il file.\n");
    printf("    se non specificato sarà 0.\n");
    printf("\n");
}