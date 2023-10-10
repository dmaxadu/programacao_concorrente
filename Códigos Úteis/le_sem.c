#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

//variáveis globais
sem_t mutex_l, mutex_e, leitores, escritores;
int l = 0, e = 0;

int main(){
    sem_init(&mutex_l, 0 , 1);
    sem_init(&mutex_e, 0 , 1);
    sem_init(&leitores, 0, 1);
    sem_init(&escritores, 0, 1);
}

void entra_leitor(){
    sem_wait(&mutex_l);
    sem_wait(&leitores);
    l++;
    if(l == 1){
        sem_wait(&escritores);
    }
    sem_post(&mutex_l);
    sem_post(&leitores);
}

void sai_leitor(){
    sem_wait(&mutex_l);
    l--;
    if(l == 0){
        sem_post(&escritores);
    }
    sem_post(&mutex_l);
}

void entra_escritor(){
    sem_wait(&mutex_e);
    e++;
    if(e == 1){
        sem_wait(&leitores);
    }
    sem_post(&mutex_e);
    sem_wait(&escritores);
}

void sai_escritor(){
    sem_post(&escritores);
    sem_wait(&mutex_e);
    e--;
    if(e == 0){
        sem_post(&leitores);
    }
    sem_post(&mutex_e);
}