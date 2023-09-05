#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>
#include"timer.h"

int primeQt = 0;
long long int N;
int n_threads;
pthread_mutex_t key;

typedef struct{
   int start;
   int end;
} tArgs;

int is_prime(long long int n){
    int i;
    if(n <= 1) return 0;
    if(n == 2) return 1;
    if((n % 2) == 0) return 0;
    for (i = 3; i < (sqrt(n) + 1); i += 2){
        if((n % i) == 0) return 0;
    }
    return 1;
}

void *prime_counter(void *arg){
    tArgs *args = (tArgs*) arg;
    int localPrimeQt = 0;

    for(long long int i = args->start; i < args->end; i++){
        if(is_prime(i)){
            localPrimeQt++;
        }
    }

    pthread_mutex_lock(&key);
    primeQt += localPrimeQt;
    pthread_mutex_unlock(&key);
        
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    pthread_t *tid; 
    tArgs *args; 
    double startTime, endTime, delta;

    if(argc<2) {
        printf("Digite: %s <N - quantidade de numeros a serem verificados de 0 a N> <numero de threads>\n", argv[0]);
        return 1;
    }

    N = atoll(argv[1]);
    n_threads = atoi(argv[2]);
    if (n_threads > N) n_threads = N;

    GET_TIME(startTime);

    tid = (pthread_t *) malloc(sizeof(pthread_t) * n_threads);
    if(tid == NULL){
        printf("Erro: MALLOC");
        return 2;
    }

    args = (tArgs *) malloc(sizeof(tArgs) * n_threads);
    if(args == NULL){
        printf("Erro: MALLOC");
        return 2;
    }

    pthread_mutex_init(&key, NULL);

    int interval_size = (int) N / n_threads;
    int current_start = 0;

    for(int i = 0; i < n_threads; i++){
        (args+i)->start = current_start;
        (args+i)->end = (i == n_threads - 1) ? N : current_start + interval_size;
        if(pthread_create(tid+i, NULL, prime_counter, (void*) (args+i))){
         printf("ERRO: pthread_create"); return 3;
        }
        current_start += interval_size;
    }

    for(int i = 0; i < n_threads; i++){
        pthread_join(*(tid+i), NULL);
    }

    GET_TIME(endTime)   
    delta = endTime - startTime;
    printf("N = %lld, n_threads = %d\n", N, n_threads);
    printf("Time of execution: %lfs\n", delta);

    free(args);
    free(tid);
    printf("Prime numbers quantity: %d\n", primeQt);

    return 0;
}