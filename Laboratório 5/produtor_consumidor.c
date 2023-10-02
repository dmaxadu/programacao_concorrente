#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

#define TAMLINHA 1000

//variáveis globais;
int n_threads;
int sinaliza_fim = 0;
char *arquivo;
char *buffer;

//semáforos para sincronização por condição e exclusão mútua;
sem_t slotVazio, slotCheio;
sem_t mutex;

//protótipo da função consumidora;
void *consumidora();

//função main, responsável por atuar como produtora e criar as threads consumidores;
int main(int argc, char* argv[]){
    pthread_t *tid;
    FILE *arquivo_texto;

    //verifica se o programa foi chamado corretamente;
    if(argc < 3){
        printf("Digite %s <numero de threads> <nome do arquivo>", argv[0]);
        return 1;
    }

    //pega os argumentos passados na linha de comando;
    n_threads = atoi(argv[1]);
    arquivo = argv[2];

    //aloca memória necessária para as threads e para o buffer que vai receber as linhas do arquivo a ser lido;
    tid = malloc(sizeof(pthread_t) * n_threads);
    if(tid == NULL){
        printf("Erro: MALLOC -- tid");
        return 2;
    }

    buffer = malloc(sizeof(char) * TAMLINHA);
    if(buffer == NULL){
        printf("Erro: MALLOC -- buffer");
        return 2;
    }

    //inicializa os semáforos;
    sem_init(&slotCheio, 0, 0);
    sem_init(&slotVazio, 0, 1);
    sem_init(&mutex, 0, 1);

    //dispara os fluxos das threads consumidoras;
    for(int i = 0; i < n_threads; i++){
        if(pthread_create(&tid[i], NULL, consumidora, NULL)){
            printf("Erro: pthread_create -- consumidora");
            return 3;
        }
    }

    /* parte produtora da main */
    //abre o arquivo e verifica se foi aberto corretamente;
    arquivo_texto = fopen(arquivo, "r");
    if(arquivo_texto == NULL){
        printf("Erro: fopen -- arquivo_texto");
        return 4;
    }

    //faz a leitura do arquivo linha por linha, e escreve no buffer;
    while(!feof(arquivo_texto)){
        //espera por um slot vazio e faz exclusão mútua;
        sem_wait(&slotVazio);
        sem_wait(&mutex);

        //pega a linha do arquivo, e escreve no buffer;
        fgets(buffer, sizeof(char) * TAMLINHA, arquivo_texto);

        //sinaliza um slot cheio e libera o mutex;
        sem_post(&slotCheio);
        sem_post(&mutex);
    }

    sinaliza_fim = 1;

    //fecha o arquivo que foi lido e libera a memória que foi alocada ao longo do programa;
    fclose(arquivo_texto);
    free(tid);
    free(buffer);
    sem_destroy(&slotCheio);
    sem_destroy(&slotVazio);
    sem_destroy(&mutex);
}

//função chamada pelas threads consumidoras disparadas na main;
void *consumidora(){
    while(!sinaliza_fim){
        //espera por algo no buffer para ser impresso;
        sem_wait(&slotCheio);

        //inicio da exclusão mútua;
        sem_wait(&mutex);

        //imprime a linha;
        printf("%s", buffer);

        //sinaliza um slot vazio;
        sem_post(&slotVazio);

        //devolve o lock e finaliza a exclusão mútua;
        sem_post(&mutex);
    }

    pthread_exit(NULL);
}