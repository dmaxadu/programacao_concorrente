#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

//variáveis globais
pthread_mutex_t mutex;
pthread_cond_t cond;
int controle = 0, n_threads;

int main(){
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
}

void barreira(){
    pthread_mutex_lock(&mutex);
    controle++;
    if(controle < n_threads){
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_cond_broadcast(&cond);
    controle = 0;
    pthread_mutex_unlock(&mutex);
}