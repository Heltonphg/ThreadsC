/*
O problema do Jantar dos filósofos, sem controle de impasses.
Compilar com o comando:
gcc -Wall filosofos.c -o filosofos -lpthread

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUMFILO 5
#define NUM_OF_ROUNDS 10000 //Filósofo irá comer por 10000 rodadas

pthread_t filosofo [NUMFILO] ;  // threads filósofos
sem_t     palito    [NUMFILO] ; // um semaforo para cada palito (iniciam em 1)

// espaços para separar as colunas de impressão
char *space[] = {"", "\t", "\t\t", "\t\t\t", "\t\t\t\t" } ;

// filósofo comendo
void come (int f){
  //printf ("%sCOMENDO\n", space[f]) ;
}

// filósofo meditando
void medita (int f){
  //printf ("%smeditando\n", space[f]) ;
}

// pega o palito
void pega_palito (int f, int h){
  //printf ("%squer h%d\n", space[f], h) ;
  sem_wait (&palito [h]) ;
  //printf ("%spegou h%d\n", space[f], h) ;
}

// larga o palito
void larga_palito (int f, int h){
  //printf ("%slarga h%d\n", space[f], h) ;
  sem_post (&palito [h]) ;
}

// corpo da thread filosofo
void *threadFilosofo (void *arg){
  int i = (long int) arg ;
  int count = NUM_OF_ROUNDS;
    while (count>=0){
    medita (i) ;
    pega_palito (i, i) ;
    int is_right_free;
    int LIVRE = 0;
    sem_getvalue(&palito [(i+1) % NUMFILO], &is_right_free);
    if(is_right_free == LIVRE){
        larga_palito (i, i);
        continue;
    }
    pega_palito (i, (i+1) % NUMFILO) ;
    sleep(0);
    come (i) ;
    larga_palito (i, (i+1) % NUMFILO) ;
    larga_palito (i, i) ;
    count--;
  }
  printf("Filosofo %d terminou!\n",i);
  pthread_exit (NULL) ;
}

// programa principal
int main (int argc, char *argv[]){
  long i, status ;

  // para o printf não se confundir com a threads
  setvbuf (stdout, 0, _IONBF, 0) ;

  // inicia os palitos
  for(i=0; i<NUMFILO; i++)
    sem_init (&palito[i], 0, 1) ;

  // inicia os filosofos
  for(i=0; i<NUMFILO; i++){
    status = pthread_create (&filosofo[i], NULL, threadFilosofo, (void *) i) ;
    if (status){
      perror ("pthread_create") ;
      exit (1) ;
    }
  }
  //Espera todas os filosofos terminarem
  for (int i = 0; i < NUMFILO; i++)
       pthread_join(filosofo[i], NULL);
  printf("\nParabéns, você resolveu o problema da espera circular! \\o/ \n");
  // a main encerra aqui
  pthread_exit (NULL) ;
}


