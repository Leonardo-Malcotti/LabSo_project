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

//controlla che il path (arg) sia una directory
//ritorna 0 se è un file, 1 se è una directory e -1 se c'è stato qualche errore
int is_dir(char * arg);

//restituisce la lungheza in byte del file con descrittore
//non esegue controlli
//ATTENZIONE: riporta il puntatore ad inizio file
int file_len(int des);

//restituisce la lunghezza del file con descrittore partendo dalla posizione pos
//non fa controlli
//riporta il puntatore a pos
int file_len2(int des,int pos);

//legge da un file con descrittore fino a che non trova un \n
//il file deve essere aperto prima di richiamare la funzione
//restituisce -1 se non è stato letto nulla, restituisce 0 se ha avuto successo
//des indica il descrittore, *buf è dove verrà salvato il contenuto, *len è la quantità di caratteri letti
//è importante passare un buf di grandezza sufficiente a contenere tutto ciò che può essere letto
int read_until_n(int des,char *buf, int *len);

//esegue il comando in command usando system(), ma salva il risultato nella pipe pip
//restituisce -1 se c'è stato qualche errore,se no da 0
int pipe_system_command(int pip[2],char *command);


#define ARG_N 0
#define ARG_M 1
#define ARG_F 2
#define ARG_C 3

#define WRITE_P 1
#define READ_P 0

#define PIPE_CHANNEL 4

#endif
