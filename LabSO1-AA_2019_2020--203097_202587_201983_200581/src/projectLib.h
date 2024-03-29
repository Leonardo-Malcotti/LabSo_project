#ifndef PROJECTLIB_H
#define PROJECTLIB_H

//stampa una guida all'utilizzo dei comandi
void print_q_help();
void print_p_help();
void print_r_help();
void print_m_help();

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

//legge da un file con descrittore fino a che non trova una corrispondenza a str
//il file deve essere aperto prima di richiamare la funzione
//restituisce -1 se non è stato letto nulla, restituisce 0 se ha avuto successo
//des indica il descrittore, *buf è dove verrà salvato il contenuto, *len è la quantità di caratteri letti, str indica il char a cui fermarsi
//mettere '\n' in str per leggere tutta una riga
//è importante passare un buf di grandezza sufficiente a contenere tutto ciò che può essere letto
int read_until_char(int des,char str,char *buf, int *len);

//esegue il comando in command usando system(), ma salva il risultato nella pipe pip
//restituisce -1 se c'è stato qualche errore,se no da 0
int pipe_system_command(int pip[2],char *command);

//conta il numero di file (o cartelle) contenuti in una cartella
//restituisce -1 se ci sono stati errori, se no da il numero di file
int n_files_in_dir(char * path);

//conta il numero di file all'interno di una gerarchia di cartelle
//non conta le cartella
int n_files_in_dir_subdir(char * path);

//restituisce il contenuto di una cartella
//ATTENZIONE: ret deve essere dimensionata correttamente, controllando preventivamente
//il numero di file all'interno della cartella
void files_in_dir(char * path,char ** ret);

//restituisce il contenuto di tutto l'albero della cartella
//path è il nome della cartella da cui partire
//buf è la matrice che deve contenere il risultato, ATTENZIONE:
//buf deve essere dimensionata per contenere tutti i percorsi
//p deve essere un int passato per riferimento che indica la posizione in cui iniziare a inserire i percorsi
void files_in_dir_subdir(char * path, int * p, char **buf);

//legge la stringa dallo stdin sostituendo eventuali caporiga con il terminatore di stringa
char * read_input();

//funzione di comode per sapere quante cifre ha un numero intero positivo
int int_len(int arg);

//aggiorna il valore di stat in base al carattere
void char_type(int *stat,int *caratteri,int j);

//controlla se il char passato è una lettera minuscola
int is_min(char j);

//restituisce un valore simbolico in base alla categoria di char
int ret_char_type(char j);


//funzioni di comodo per stampare le tabelle del report
void r_stampa_tutto(int *valori);

void r_stampa_consonanti(int *valori);

void r_stampa_vocali(int *valori);

void r_stampa_numeri(int *valori);

void r_stampa_punteggiatura(int *valori);

char * dir_corrente();

#define ARG_N 0
#define ARG_M 1
#define ARG_F 2
#define ARG_C 3

#define WRITE_P 1
#define READ_P 0

#define PIPE_CHANNEL 4
#define BUFFER_IN_LEN 256
#define N_CARATTERI 128

#define TOTALE 0
#define MAIUSCOLE 1
#define MINUSCOLE 2
#define CONSONANTI 3
#define VOCALI 4
#define PUNTEGGIATURA 5
#define SPECIALI 6
#define NUMERI 7


#endif
