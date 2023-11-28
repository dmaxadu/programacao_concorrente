#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int n_threads; //variavel global que armazena a quantidade de threads a serem utilizadas;
int aux = 0; //variável auxiliar que será utilizada para verificar se a condição foi satisfeita;
pthread_mutex_t mutex; //variável de lock a ser utilizada na exclusão mútua;
pthread_cond_t cond; //variável condicional que será usada para travar as threads se a condição não for satisfeita;

//protótipos das funções barreira e tarefa;
void barreira();

void *tarefa(void *id);

//função main;
int main(int argc, char* argv[]){
    pthread_t *tid; //identificadores das threads;

    //se a quantidade de argumentos na linha de comando for menor que dois, então o N não foi passado.
    if(argc<2) {
        printf("Digite: %s <numero de threads>\n", argv[0]);
        return 1;
    }

    n_threads = atoi(argv[1]);

    //aloca memória para os identificadores das threads;
    tid = (pthread_t *) malloc(sizeof(pthread_t) * n_threads);

    //verifica se ocorreu algum erro na alocação de memória;
    if(tid == NULL){
        printf("Erro: MALLOC");
        return 2;
    }

    //inicializa as variáveis mutex e cond;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    //dispara as threads;
    for(long int i = 0; i < n_threads; i++){
        if(pthread_create(tid + i, NULL, tarefa, (void *)i)){
            printf("Erro: pthread_create");
            return 3;
        }
    }

    //espera pelo término das threads para finalizar o programa;
    for(int i = 0; i < n_threads; i++){
        pthread_join(*(tid + i), NULL);
    }

    //liberação de memória;
    free(tid);

    //destruição das varáveis mutex e cond;
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}

//função para realizar a sincronização por barreira;
void barreira(){
    //início da seção crítica, onde será utilizado a exclusão mútua;
    pthread_mutex_lock(&mutex);
    aux++;
    //se for diferente, ainda faltam threads, a thread em questão se bloqueia até que todas cheguem aqui;
    if(aux != n_threads){
        pthread_cond_wait(&cond, &mutex);
    }
    //se aux = n_threads, então todas as threads estão no mesmo ponto, sendo assim, a última desbloqueia todas as threads;
    else {
        pthread_cond_broadcast(&cond);
        aux = 0;
    }
    //fim da seção crítica
    pthread_mutex_unlock(&mutex);
}

//função tarefa que será realizada por cada thread;
void *tarefa(void *id){
    int id_local = (long int) id + 1;
    printf("Olá da thread %d\n", id_local);
    barreira();

    printf("Que dia bonito da thread %d\n", id_local);
    barreira();

    printf("Até breve da thread %d\n", id_local);
    barreira();

    pthread_exit(NULL);
}
