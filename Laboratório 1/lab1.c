#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N_THREADS 8
#define VECTOR_SIZE 10000
#define ERROR_MALLOC "Erro: Não foi possível alocar a memória necessária."
#define ERROR_PTHREAD_CREATE "Erro ao criar a Thread."
#define ERROR_PTHREAD_JOIN "Erro ao dar join no resultado."
#define ERROR 0
#define SUCCESS 1

/*argumentos utilizados na função square_num*/
typedef struct{
    int i_thread, n_thread, *v, vector_size;
} t_Args;

/*eleva ao quadrado os elementos do vetor*/
void *square_num(void *arg){
    t_Args args = *(t_Args *)arg;
    int i = args.i_thread;

    for(; i < args.vector_size; i += args.n_thread){
        args.v[i] = args.v[i] * args.v[i];
    }

    free(arg);
    pthread_exit(NULL);
}

/*declara e inicializa um vetor de inteiros*/
int *initialize_vector(){
    int *vector = (int *)malloc(VECTOR_SIZE * sizeof(int));
    
    if(vector == NULL){
        printf(ERROR_MALLOC);
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < VECTOR_SIZE; i++){
        vector[i] = i + 1;
    }

    return vector;
}

/*verifica se todos os elementos do vetor foram elevados ao quadrado corretamente*/
int verify_vector(int *vector){
    
    for(int i = 0; i < VECTOR_SIZE; i++){
        if(vector[i] != ((i + 1) * (i + 1))){
            
            return ERROR;
        }
    }

    return SUCCESS;
}

void print_vector(int *vector){
    int max_prints = 50;
    int j;
    for(j = 0; j < max_prints; j++){
        printf("\nElemento %d: %d", j + 1, vector[j]);
    }
}

int main(){
    int *vector = initialize_vector();
    pthread_t threads[N_THREADS];
    t_Args *args;

    for(int i = 0; i < N_THREADS; i++){
        args = malloc(sizeof(t_Args));
        if(args == NULL){
            printf(ERROR_MALLOC);
            exit(EXIT_FAILURE);
        }

        args->i_thread = i;
        args->n_thread = N_THREADS;
        args->v = vector;
        args->vector_size = VECTOR_SIZE;

        if(pthread_create(&threads[i], NULL, square_num, (void *) args)){
            printf(ERROR_PTHREAD_CREATE);
            exit(EXIT_FAILURE);
        }
    }

    for(int i = 0; i < N_THREADS; i++){
        if(pthread_join(threads[i], NULL)){
            printf(ERROR_PTHREAD_JOIN);
            exit(EXIT_FAILURE);
        }
    }

    if(verify_vector(vector) == 1){
        printf("Todos os valores foram elevados ao quadrado corretamente!");
        print_vector(vector);
    } else{
        printf("Foi encontrado um erro, tente novamente.");
    }
}