//Programa concorrente que cria e faz operacoes sobre uma lista de inteiros

#include <stdio.h>
#include <stdlib.h>
#include "list_int.h"
#include <pthread.h>
#include "timer.h"

#define QTDE_OPS 10000000 //quantidade de operacoes sobre a lista (insercao, remocao, consulta)
#define QTDE_INI 100 //quantidade de insercoes iniciais na lista
#define MAX_VALUE 100 //valor maximo a ser inserido

//lista compartilhada iniciada 
struct list_node_s* head_p = NULL; 
//qtde de threads no programa
int nthreads;

//rwlock de exclusao mutua
pthread_rwlock_t rwlock;

//tarefa das threads
void* tarefa(void* arg) {
   long int id = (long int) arg;
   int op;
   int in, out, read; 
   in=out=read = 0; 

   //realiza operacoes de consulta (98%), insercao (1%) e remocao (1%)
   for(long int i=id; i<QTDE_OPS; i+=nthreads) {
      op = rand() % 100;
      if(op<98) {
	 pthread_rwlock_rdlock(&rwlock); /* lock de LEITURA */    
         Member(i%MAX_VALUE, head_p);   /* Ignore return value */
	 pthread_rwlock_unlock(&rwlock);     
	 read++;
      } else if(98<=op && op<99) {
	 pthread_rwlock_wrlock(&rwlock); /* lock de ESCRITA */    
         Insert(i%MAX_VALUE, &head_p);  /* Ignore return value */
	 pthread_rwlock_unlock(&rwlock);     
	 in++;
      } else if(op>=99) {
	 pthread_rwlock_wrlock(&rwlock); /* lock de ESCRITA */     
         Delete(i%MAX_VALUE, &head_p);  /* Ignore return value */
	 pthread_rwlock_unlock(&rwlock);     
	 out++;
      }
   }
   //registra a qtde de operacoes realizadas por tipo
   printf("Thread %ld: in=%d out=%d read=%d\n", id, in, out, read);
   pthread_exit(NULL);
}

/*-----------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   pthread_t *tid;
   double ini, fim, delta;
   
   //verifica se o numero de threads foi passado na linha de comando
   if(argc<2) {
      printf("Digite: %s <numero de threads>\n", argv[0]); return 1;
   }
   nthreads = atoi(argv[1]);

   //insere os primeiros elementos na lista
   for(int i=0; i<QTDE_INI; i++)
      Insert(i%MAX_VALUE, &head_p);  /* Ignore return value */
   
   //tomada de tempo inicial
   GET_TIME(ini);

   //aloca espaco de memoria para o vetor de identificadores de threads no sistema
   tid = malloc(sizeof(pthread_t)*nthreads);
   if(tid==NULL) {  
      printf("--ERRO: malloc()\n"); return 2;
   }

   //inicializa a variavel mutex
   pthread_rwlock_init(&rwlock, NULL);
   
   //cria as threads
   for(long int i=0; i<nthreads; i++) {
      if(pthread_create(tid+i, NULL, tarefa, (void*) i)) {
         printf("--ERRO: pthread_create()\n"); return 3;
      }
   }
   
   //aguarda as threads terminarem
   for(int i=0; i<nthreads; i++) {
      if(pthread_join(*(tid+i), NULL)) {
         printf("--ERRO: pthread_join()\n"); return 4;
      }
   }

   //tomada de tempo final
   GET_TIME(fim);
   delta = fim-ini;
   printf("Tempo: %lf\n", delta);

   //libera o mutex
   pthread_rwlock_destroy(&rwlock);
   //libera o espaco de memoria do vetor de threads
   free(tid);
   //libera o espaco de memoria da lista
   Free_list(&head_p);

   return 0;
}  /* main */

