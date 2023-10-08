#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<string.h>

#define TAMLINHA 1000
#define TAMBUFFER 5

//variáveis globais;
int n_threads;
int linhas_lidas = 0, linhas_printadas = 0, sinaliza_fim = 0, threads_ativas = 0;
char buffer[TAMBUFFER][TAMLINHA];

//semáforos para sincronização por condição e exclusão mútua;
sem_t slotVazio, slotCheio, threads_concluidas;
sem_t mutexProd, mutexCons, mutex;

void insere(char *linha);
void retira();
void produtora(char *linha);
void *consumidora();

int main(int argc, char const *argv[]){
    pthread_t *tid;
    FILE *arquivo_texto;
    char linha_aux[TAMLINHA];

    //verifica se o programa foi chamado corretamente;
    if(argc < 3){
        printf("Digite %s <numero de threads> <nome do arquivo>", argv[0]);
        return 1;
    }

    //pega os argumentos passados na linha de comando;
    n_threads = atoi(argv[1]);
    arquivo_texto = fopen(argv[2], "r");
    if(arquivo_texto == NULL){
        printf("Erro ao abrir o arquivo");
        return 3;
    }

    //aloca memória necessária para as threads e para o buffer que vai receber as linhas do arquivo a ser lido;
    tid = malloc(sizeof(pthread_t) * n_threads);
    if(tid == NULL){
        printf("Erro: MALLOC -- tid");
        return 2;
    }

    //inicializa os semáforos;
    sem_init(&slotCheio, 0, 0);
    sem_init(&slotVazio, 0, TAMBUFFER);
    sem_init(&threads_concluidas, 0, 0);
    sem_init(&mutex, 0, 1);

    for(int i = 0; i < n_threads; i++){
        threads_ativas++;
        if(pthread_create(&tid[i], NULL, consumidora, NULL)){
            printf("Erro: pthread_create -- consumidora");
            return 3;
        }
    }

    while(fgets(linha_aux, sizeof(char) * TAMLINHA, arquivo_texto) != NULL){
        produtora(linha_aux);
    }
    
    for(int i = 0; i < n_threads; i++){
        sem_wait(&threads_concluidas);
    }

    sinaliza_fim = 1;
    for (int i = 0; i < n_threads; i++) {
        sem_post(&slotVazio); // Libere quaisquer threads bloqueadas no semáforo slotVazio
    }

    while(threads_ativas > 0);

    for(int i = 0; i < n_threads; i++) {
        pthread_join(*(tid+i), NULL);
    }

    fclose(arquivo_texto);
    free(tid);
    sem_destroy(&slotCheio);
    sem_destroy(&slotVazio);
    sem_destroy(&threads_concluidas);
    sem_destroy(&mutex);

    return 0;
}

void insere(char *linha){
    static int in = 0;
    strcpy(buffer[in], linha);
    //printf("Inseri: %s", buffer[in]); apenas para debug
    in = (in + 1) % TAMBUFFER;
    linhas_lidas++;
}

void retira(){
    static int out = 0;
    char linha_aux[TAMLINHA];
    strcpy(linha_aux, buffer[out]);
    printf("%s", linha_aux);
    out = (out + 1) % TAMBUFFER;
    linhas_printadas++;
}

void produtora(char *linha){
    sem_wait(&slotVazio);
    sem_wait(&mutex);
    insere(linha);
    sem_post(&slotCheio);
    sem_post(&mutex);
}

void *consumidora(){
    while (1) {
        sem_wait(&slotCheio);
        sem_wait(&mutex);

        // Verifica se todas as linhas foram impressas antes de sair
        if (sinaliza_fim && linhas_printadas >= linhas_lidas) {
            threads_ativas--;
            sem_post(&mutex); // Libere o mutex antes de sair
            sem_post(&threads_concluidas); // Sinalize que esta thread terminou
            pthread_exit(NULL);
        }

        retira();
        sem_post(&slotVazio);
        sem_post(&mutex);
    }
}