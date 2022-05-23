/*

Acessos concorrentes a uma variável por múltiplias threads. Não há controle da entrada e saída da região crítica.
    
Compilar com:

gcc -Wall regiao_critica_trivial.c -o regiao_critica_trivial -lpthread
*/
     
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
     
#define NUM_THREADS  10
#define NUM_OPERATIONS 1000
     
int value = NUM_THREADS*NUM_OPERATIONS;

int busy = 0 ; //define a busy variable to be used for critical section protection.
 
void enter_cs (){
  while ( busy ) ;   // busy waiting
  busy = 1 ;
}
 
void leave_cs (){
  busy = 0 ;
}

     
void *threadBody (void *id){
       int i ;
       for (i=0; i< NUM_OPERATIONS; i++){
       	  enter_cs ((long int) id) ; // 
          value -= 1 ;   // critical section
          leave_cs () ;
       }
       pthread_exit (NULL) ;
}
     
int main (int argc, char *argv[]){
       pthread_t thread [NUM_THREADS] ;
       pthread_attr_t attr ;
       long i, status ;
     
       pthread_attr_init (&attr) ;
       pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE) ;
     
       // create threads
       for(i=0; i<NUM_THREADS; i++){
          status = pthread_create (&thread[i], &attr, threadBody, (void *) i) ;
          if (status)
          {
             perror ("pthread_create") ;
             exit (1) ;
          }
       }
     
       // wait all threads to finish   
       for (i=0; i<NUM_THREADS; i++)
       {
          status = pthread_join (thread[i], NULL) ;
          if (status)
          {
             perror ("pthread_join") ;
             exit (1) ;
          }
       }
     
       printf ("Value should be %d and is %d\n", 0, value) ;
     
       pthread_attr_destroy (&attr) ;
       pthread_exit (NULL) ;
}


