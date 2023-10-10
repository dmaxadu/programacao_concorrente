#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

#define TAM_BUFFER 5

//variáveis globais
sem_t mutex, prod, cons;
int buffer[TAM_BUFFER], slots_cheios = 0;

int main(){
    sem_init(&mutex, 0, 1);
    sem_init(&prod, 0, 1);
    sem_init(&cons, 0, 1);
}

void insere(int e){
    static int in = 0;
    sem_wait(&prod);
    sem_wait(&mutex);
    
    buffer[in] = e;
    in = (in + 1) % TAM_BUFFER;

    sem_post(&cons);
    sem_post(&mutex);
}
int remove(){
    static int out = 0;
    int e = 0;
    sem_wait(&cons);
    sem_wait(&mutex);

    e = buffer[out];
    out = (out + 1) % TAM_BUFFER;

    sem_post(&prod);
    sem_post(&mutex);
}
//mesmas funções do pc_cond.c
void *produtor();
void *consumidor();