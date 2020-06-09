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

int main(int argc, char *argv[]) {
    char caratteri1 [33][4] = {"nul","soh","stx","etx","eot","enq","ack","bel","bs","ht","nl","vt","ff","cr","so","si","dle","dc1","dc2","dc3","dc4","nak","syn","etb","can","em","sub","esc","fs","gs","rs","us","sp"};
    int i,x;//indice per i for
    int finput,len_file;

    //primi controlli sui parametri
    if((finput=open_file("report.txt",&len_file))==-1){
                //stampa errori
        exit(-1);
    }

    //legge il file e stampa a mo' di tabella
    //a sinistra il valore ascii a destra la quantità e percentuale
    printf("REPORT\n\nGeneral statistics:\n");
    printf("\033[0;31m");
    for(x=0;x<8;x++){
      printf("| char ");
      printf("  freq ");
      printf("    %% |");
    }
    printf("\033[0m");
    printf("\n");
    char buf[100];
        strcpy(buf,"");
        int contr,len;
        int j;
        int tmp;
	for(i=0;i<16;i++){
        //divide in 8 righe
        for(j=0;j<8;j++){
            //legge il primo valore, che è prima dello spazio
            contr = read_until_char(finput,' ',buf,&len);
            if(contr<0){
                //errore nella lettura
                exit(-1);
            }
            tmp=str_to_int(buf);
            if(tmp<33){
              printf("|%5s ",caratteri1[tmp]);
            }
            else{
              printf("|%5c ",tmp);
            }
            strcpy(buf,"");
            contr = read_until_char(finput,' ',buf,&len);
            if(contr<0){
                //errore nella lettura
                exit(-1);
            }
            //i+=len+1;
            tmp=str_to_int(buf);
            printf(" %5d ",tmp);
            strcpy(buf,"");
            //legge il secondo valore, che è prima della fine della riga
            contr = read_until_char(finput,'\n',buf,&len);
            if(contr<0){
                //errore nella lettura
                exit(-1);
            }
            //i+=len+1;
            printf(" %5.2f |",atof(buf));
            strcpy(buf,"");

        }
        printf("\n");
	}
  //stampa statistiche su categorie di caratteri
  printf("Categorie caratteri:\n");
  printf("\033[0;31m");
  printf("    categoria ");
  printf("  freq ");
  printf("    %% ");
  printf("\033[0m");
  printf("\n");
  for(j=0;j<8;j++){
      //legge il primo valore, che è prima dello spazio
      contr = read_until_char(finput,' ',buf,&len);
      if(contr<0){
          //errore nella lettura
          exit(-1);
      }
      printf("\033[0;32m");
      printf("%13s\t",buf);
      printf("\033[0m");
      strcpy(buf,"");
      contr = read_until_char(finput,' ',buf,&len);
      if(contr<0){
          //errore nella lettura
          exit(-1);
      }
      //i+=len+1;
      tmp=str_to_int(buf);
      printf(" %2d ",tmp);
      strcpy(buf,"");
      //legge il secondo valore, che è prima della fine della riga
      contr = read_until_char(finput,'\n',buf,&len);
      if(contr<0){
          //errore nella lettura
          exit(-1);
      }
      //i+=len+1;
      printf("\t%3.2f\n",atof(buf));
      strcpy(buf,"");

  }
  printf("\n");
  
  int bool=1;
  if(argc>1){
    while(read_until_char(finput,'\n',buf,&len)!=-1){
      i=2;
      while(bool!=0 || i<argc){
        if(strcmp(buf,argv[i])==0)
        {
          bool=0;
          printf("%s\n",argv[i]);
          printf("\033[0;31m");
          for(x=0;x<8;x++){
            printf("| char ");
            printf("  freq ");
            printf("    %% |");
          }
          printf("\033[0m");
          printf("\n");
          char buf[100];
              strcpy(buf,"");
              int contr,len;
              int j;
              int tmp;
      	  for(i=0;i<16;i++){
              //divide in 8 righe
              for(j=0;j<8;j++){
                  //legge il primo valore, che è prima dello spazio
                  contr = read_until_char(finput,' ',buf,&len);
                  if(contr<0){
                      //errore nella lettura
                      exit(-1);
                  }
                  tmp=str_to_int(buf);
                  if(tmp<33){
                    printf("|%5s ",caratteri1[tmp]);
                  }
                  else{
                    printf("|%5c ",tmp);
                  }
                  strcpy(buf,"");
                  contr = read_until_char(finput,' ',buf,&len);
                  if(contr<0){
                      //errore nella lettura
                      exit(-1);
                  }
                  //i+=len+1;
                  tmp=str_to_int(buf);
                  printf(" %5d ",tmp);
                  strcpy(buf,"");
                  //legge il secondo valore, che è prima della fine della riga
                  contr = read_until_char(finput,'\n',buf,&len);
                  if(contr<0){
                      //errore nella lettura
                      exit(-1);
                  }
                  //i+=len+1;
                  printf(" %5.2f |",atof(buf));
                  strcpy(buf,"");

              }
              printf("\n");
      	}
        //stampa statistiche su categorie di caratteri
        printf("Categorie caratteri:\n");
        printf("\033[0;31m");
        printf("    categoria ");
        printf("  freq ");
        printf("    %% ");
        printf("\033[0m");
        printf("\n");
        for(j=0;j<8;j++){
            //legge il primo valore, che è prima dello spazio
            contr = read_until_char(finput,' ',buf,&len);
            if(contr<0){
                //errore nella lettura
                exit(-1);
            }
            printf("\033[0;32m");
            printf("%13s\t",buf);
            printf("\033[0m");
            strcpy(buf,"");
            contr = read_until_char(finput,' ',buf,&len);
            if(contr<0){
                //errore nella lettura
                exit(-1);
            }
            //i+=len+1;
            tmp=str_to_int(buf);
            printf(" %2d ",tmp);
            strcpy(buf,"");
            //legge il secondo valore, che è prima della fine della riga
            contr = read_until_char(finput,'\n',buf,&len);
            if(contr<0){
                //errore nella lettura
                exit(-1);
            }
            //i+=len+1;
            printf("\t%3.2f\n",atof(buf));
            strcpy(buf,"");

        }
        printf("\n");
        }
        i++;
      }


    }
  }
    close(finput);
    return 0;
}
