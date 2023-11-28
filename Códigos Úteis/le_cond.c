#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

//variáveis globais
pthread_mutex_t mutex;
pthread_cond_t leitor, escritor;
int l = 0, e = 0;

int main(){
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&leitor, NULL);
    pthread_cond_init(&escritor, NULL);
}

void entra_leitor(){
    pthread_mutex_lock(&mutex);
    while(e > 0){
        pthread_cond_wait(&leitor, &mutex);
    }
    l++;
    pthread_mutex_unlock(&mutex);
}

void sai_leitor(){
    pthread_mutex_lock(&mutex);
    l--;
    if(l == 0){
        pthread_cond_signal(&escritor);
    }
    pthread_mutex_unlock(&mutex);
}

void entra_escritor(){
    pthread_mutex_lock(&mutex);
    while(e > 0 || l > 0){
        pthread_cond_wait(&escritor, &mutex);
    }
    e++;
    pthread_mutex_unlock(&mutex);
}

void sai_escritor(){
    pthread_mutex_lock(&mutex);
    e--;
    if(e == 0){
        pthread_cond_broadcast(&leitor);
    }
    else {
        pthread_cond_signal(&escritor);
    }
    pthread_mutex_unlock(&mutex);
}