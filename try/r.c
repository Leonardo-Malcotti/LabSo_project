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

int char_type(char c);

int main(int argc, char *argv[]) {
    char caratteri1 [33][3] = {"nul","soh","stx","etx","eot","enq","ack","bel","bs","ht","nl","vt","ff","cr","so","si","dle","dc1","dc2","dc3","dc4","nak","syn","etb","can","em","sub","esc","fs","gs","rs","us","sp"};
    int i;//indice per i for
    int finput,len_file;
    int c_nums=0, c_lowl=0, c_uppl=0, c_spun=0, c_cpun=0, c_oths=0;

    //primi controlli sui parametri
    if(argc != 3){
        printf("utilizzo erroneo dei parametri\n");
        print_r_help();
        exit(-1);
    }

    //r accetta solo il parametro -f, con un valore che deve essere un file
    if(strcmp(argv[1],"-f") == 0){
        if((finput=open_file(argv[2],&len_file))==-1){
            //stampa errori
            exit(-1);
        }
    }

    //legge il file e stampa a mo' di tabella
    //a sinistra il valore ascii a destra la quantità
    //i valori letti vengono convertiti in int perchè hanno una formattazione più ordinata
	for(i=0;i<len_file;i+=0){
		char buf[len_file];
        	strcpy(buf,"");
        	int contr,len;
        	int j;
        	int tmp;

        	//divide in 8 righe
	        for(j=0;j<8;j++){ 
	            //legge il primo valore, che è prima dello spazio
	            contr = read_until_char(finput,' ',buf,&len);
	            if(contr<0){
	                //errore nella lettura
	                exit(-1);
	            }
	            i+=len+1;
	            tmp=str_to_int(buf);
		    int type = char_type(buf);
	            printf(" |%5d ",tmp);
	            strcpy(buf,"");
	            //legge il secondo valore, che è prima della fine della riga
	            contr = read_until_char(finput,'\n',buf,&len);
	            if(contr<0){
	                //errore nella lettura
	                exit(-1);
	            }
	            i+=len+1;
	            tmp=str_to_int(buf);
		    switch(type){
			case 0: c_nums += tmp; break;
			case 1: c_lowl += tmp; break;
			case 2: c_uppl += tmp; break;
			case 3: c_spun += tmp; break;
			case 4: c_cpun += tmp; break;
			default: c_oths += tmp; break;
		    }
	            printf(" %5d| ",tmp);
	            strcpy(buf,"");
	        }
	        printf("\n");
	}
	//USANDO LE STREAM DOVREBBE ESSERE SEMPLICE, CON I FILE DESCRIPTOR È UNA BESTEMMIA INUTILE
	printf("\nNumeri trovati:\t%d\nCaratteri minuscoli:\t%d\nCaratteri maiuscoli:\t%d\nPunteggiatura 'semplice':\t%d\nPunteggiatura 'complessa':\t%d\nAltro:\t%d\n",c_nums,c_lowl,c_uppl,c_spun,c_cpun,c_oths);
    close(finput);
    return 0;
}


int char_type(char c){
/*Simple function to detect what type of char is the one passed.
* returns:
*	- 0 : number
*	- 1 : lowercase char
*	- 2 : uppercase char
* 	- 3 : simple punctuation (period, comma, question mark, exclamation point, semicolon, colon, aposthrophe, quotation marks, ampersand) 
*	- 4 : complex punctuation (brackets, braces, parentheses, dash, @, and many others)
*	- 5 : others
*/
	int ret = 5;
	int x = c;
	if(x>47 && x<58)  { ret = 0; }else
	if(x>96 && x<123) { ret = 1; }else
	if(x>64 && x<91)  { ret = 2; }else
	if(x>31 && x<127) {
		if(x==33 || x==34 || x==39 || x==44 || x==46 || x==58 || x==59 || x==63){
			ret = 3;
		}else{
			ret = 4;
		}
	}
	return ret;
}
