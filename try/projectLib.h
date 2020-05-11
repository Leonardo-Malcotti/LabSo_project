#ifndef PROJECTLIB_H
#define PROJECTLIB_H

//stampa una guida all'utilizzo dei comandi
void print_q_help();
void print_p_help();
//verifica che la stringa passara (arg) corrisponda al parametro specificato (arg_type)(vedere arr_param)
//controlla che il parametro non venga usato più volte (arr_check)
//ritorna -1 ad errore, 0 a successo
int param_check(char *arg,int arg_type,int arr_check[]);
//converte una stringa in un int positivo
//da errore se la stringa non è parsabile o corrisponde ad un int negativo
//ritorna -1 ad errore, il numero se ha successo
int str_to_int(char * arg);
//apre il file specificato (arg) e salva la lunghezza del contenuto (len)
//ritorna -1 ad errore, il descrittore del file se ha successo
int open_file(char * arg,int *len);

#define ARG_N 0
#define ARG_M 1
#define ARG_F 2
#define ARG_C 3

#define WRITE_P 1
#define READ_P 0

#define PIPE_CHANNEL 4

#endif
