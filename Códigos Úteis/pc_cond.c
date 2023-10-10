#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define TAM_BUFFER 5

//variáveis globais
pthread_mutex_t mutex;
pthread_cond_t prod, cons;
int slots_cheios = 0;
int buffer[TAM_BUFFER];

void insere(int e){
    static int in = 0;
    pthread_mutex_lock(&mutex);
    while(slots_cheios == TAM_BUFFER){
        pthread_cond_wait(&prod, &mutex);
    }
    buffer[in] = e;
    slots_cheios++;
    in = (in + 1) % TAM_BUFFER;
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cons);
}
int remove(){
    static int out = 0;
    int e = 0;
    pthread_mutex_lock(&mutex);
    while(slots_cheios == 0){
        pthread_cond_wait(&cons, &mutex);
    }
    slots_cheios--;
    e = buffer[out];
    out = (out + 1) % TAM_BUFFER;
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&prod);
}
void *produtor(){
    for(int i = 0; i < 100; i++){
        insere(i);
    }

    pthread_exit(NULL);
}
void *consumidor(){
    int e = 0;
    for(int i = 0; i < 100; i++){
        e = remove();
        printf("%d", e);
    }
}