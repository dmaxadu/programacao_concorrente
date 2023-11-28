#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//variáveis globais para exclusão mútua, bloqueio, leitura e escrita;
pthread_mutex_t mutex;
pthread_cond_t cond_leitura;
pthread_cond_t cond_escrita;
int lendo = 0;
int escrevendo = 0;

//funções para thread leitora;
void entra_leitura(){
    //área de exclusão mútua;
    pthread_mutex_lock(&mutex);
    //enquanto uma está escrevendo, ninguém pode ler;
    while(escrevendo > 0) {
        //se alguém estiver escrevendo, eu me bloqueio e espero na fila;
        pthread_cond_wait(&cond_leitura, &mutex);
    }
    //se não tiver ninguém escrevendo, então pode ler;
    lendo++;
    pthread_mutex_unlock(&mutex);
    //printf("Threads lendo: %d\n", lendo);
}

void sai_leitura(){
    //área de exclusão mútua;
    pthread_mutex_lock(&mutex);
    lendo--;
    //se uma estiver lendo, ninguém pode escrever;
    if(lendo == 0){
        //se ninguém estiver lendo, então libera a escrita;
        pthread_cond_signal(&cond_escrita);
    }
    pthread_mutex_unlock(&mutex);
    //printf("Escrevendo: %d\nLendo: %d\n", escrevendo, lendo);
}

//funções para thread escritora;
void entra_escrita(){
    //área de exclusão mútua;
    pthread_mutex_lock(&mutex);
    //enquanto alguém estiver lendo, ou escrevendo, ninguém pode escrever;
    while(lendo > 0 || escrevendo > 0){
        pthread_cond_wait(&cond_escrita, &mutex);
    }
    //se ninguém estiver lendo ou escrevendo, então pode escrever;
    escrevendo++;
    pthread_mutex_unlock(&mutex);
    //printf("Threads escrevendo: %d", escrevendo);
}

void sai_escrita(){
    //área de exclusão mútua;
    pthread_mutex_lock(&mutex);
    escrevendo--;
    //como não tem ninguém escrevendo, libero a escrita e a leitura;
    pthread_cond_signal(&cond_escrita);
    pthread_cond_broadcast(&cond_leitura);
    pthread_mutex_unlock(&mutex);
    //printf("Escrevendo: %d\nLendo: %d\n", escrevendo, lendo);
}

//função a ser utilizada pela biblioteca para inicializar as variáveis de bloqueio e condição;
void init_vars(){
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_escrita, NULL);
    pthread_cond_init(&cond_leitura, NULL);
}

//função a ser utilizada pela biblioteca para liberar as variáveis de bloqueio e condição;
void destroy_vars(){
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_escrita);
    pthread_cond_destroy(&cond_leitura);
}