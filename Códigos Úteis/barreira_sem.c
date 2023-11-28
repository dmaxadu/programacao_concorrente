#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

//variáveis globais
sem_t mutex, cond;
int controle = 0, n_threads;

int main(){
    sem_init(&mutex, 0, 1);
    sem_init(&cond, 0, 0);
}

void barreira(){
    sem_wait(&mutex);
    controle++;
    if(controle < n_threads){
        sem_post(&mutex);
        sem_wait(&cond);
        controle--;
        if(controle > 0){
            sem_post(&cond);
        }
        else {
            sem_post(&mutex);
        }
    }
    else {
        controle--;
        sem_post(&cond);
    }
}