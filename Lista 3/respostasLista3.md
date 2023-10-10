# :zap: Lista 3 - Programação Concorrente

Daniel Machado da Conceição - 121046088

---

### :hash: Questão 1
- **(a)** O que pode ter acontecido é que a primeira thread liberada na primeira barreira, utilizou o sinal da segunda thread bloqueada na primeira barreira para se liberar na segunda barreira. Desta maneira, ela consegue passar pela segunda barreira sem que todas as threads tenham chegado ainda. O erro está no for que incrementa o semáforo utilizado como variável de condição, pois ele libera todos os sinais de uma vez.

- **(b)**
~~~c
sem_t mutex; //exclusão mútua (iniciado com 1)
sem_t cond; //condicional (iniciado com 0)
int chegaram = 0; //variável de estado global

void barreira(int numThreads){
    sem_wait(&mutex);
    chegaram++;
    if(chegaram < numThreads){
        sem_post(&mutex);
        sem_wait(&cond);
    }
}
~~~

---

### :hash: Questão 2

- **(a)** Sim, a solução está correta e atende a todos os requisitos do problema. A função *leitor()* bloqueia os escritores quando temos o primeiro leitor e todos depois dele podem ler com a garantia de que não há ninguém escrevendo naquele momento. Quando o último leitor acaba, ele libera a escrita. Caso um escritor queira escrever, ele aguarda o sinal do último leitor, e como só temos um sinal, apenas um escritor pode escrever.

---

### :hash: Questão 3

~~~c
#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>

#DEFINE TAM_BUFFER 5

//variáveis globais
int n_threads, slotsCheios = 0, slotsVazios = TAM_BUFFER;
pthread_mutex_t mutex;
pthread_cond_t cons, prod;

void insere(int e);
int retira();
void *produtor();
void *consumidor();

int main(){
    pthread_t *tid;

    tid = (pthread_t *)malloc(sizeof(pthread_t) * n_threads);
    if(tid == NULL){
        printf("Erro: malloc -- tid");
        return 2;
    }

    buffer = (int *)malloc(sizeof(int) * TAM_BUFFER);
    if(buffer == NULL){
        printf("Erro: malloc -- buffer");
        return 2;
    }

    pthread_mutex_init(&mutex);
    pthread_cond_init(&cons);
    pthread_cond_init(&prod);

    for(int i = 0; i < n_threads / 2; i++){
        if(pthread_create(tid+i, NULL, produtor, NULL)){
            printf("Erro: pthread_create -- produtor");
        }
    }

    for(int i = n_threads / 2; i < n_threads; i++){
        if(pthreade_create(tid+i, NULL, consumidor, NULL)){
            printf("Erro: pthread_create -- consumidor");
        }
    }

    for(int i = 0; i < n_threads; i++){
        pthread_join(*(tid+i), NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cons);
    pthread_cond_destroy(&prod);
    free(tid);
    free(buffer);
}

void insere(int e){
    static int in = 0;
    pthread_mutex_lock(&mutex);
    while(slotsCheios == TAM_BUFFER){
        pthread_cond_wait(&prod, &mutex);
    }
    buffer[in] = e;
    printf("Inseri: %d", e);
    in = (in + 1) % TAM_BUFFER;
    slotsCheios++;
    pthread_cond_signal(&cons);
    pthread_mutex_unlock(&mutex);
}

int retira(){
    static int out = 0;
    int e = 0;
    pthread_mutex_lock(&mutex);
    while(slotsVazios == TAM_BUFFER){
        pthread_cond_wait(&cons, &mutex);
    }

    e = buffer[out];
    out = (out + 1) % TAM_BUFFER;
    slotsCheios--;
    pthread_cond_signal(&prod);
    pthread_mutex_unlock(&mutex);
    return e;
}

void *produtor(){
    for(int i = 0; i < 100; i++){
        insere(i);
    }
    pthread_exit(NULL);
}

void *consumidor(){
    int valor = 0;
    for(int i = 0; i < 100; i++){
        valor = retira();
        printf("Retirei: %d", valor);
    }

    pthread_exit(NULL);
}
~~~

---

### :hash: Questão 4

### **Não Finalizada**

~~~c
#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

#DEFINE TAM_BUFFER 5

//variáveis globais
int n_threads, slotsCheios = 0, slotsVazios = TAM_BUFFER;
sem_t cons, prod, mutex;

void insere(int e);
int retira();
void *produtor();
void *consumidor();

int main(){
    pthread_t *tid;

    tid = (pthread_t *)malloc(sizeof(pthread_t) * n_threads);
    if(tid == NULL){
        printf("Erro: malloc -- tid");
        return 2;
    }

    buffer = (int *)malloc(sizeof(int) * TAM_BUFFER);
    if(buffer == NULL){
        printf("Erro: malloc -- buffer");
        return 2;
    }

    sem_init(&mutex, 0, 1);
    sem_init(&cons, 0, 1);
    sem_init(&prod, 0, 1);

    for(int i = 0; i < n_threads / 2; i++){
        if(pthread_create(tid+i, NULL, produtor, NULL)){
            printf("Erro: pthread_create -- produtor");
        }
    }

    for(int i = n_threads / 2; i < n_threads; i++){
        if(pthreade_create(tid+i, NULL, consumidor, NULL)){
            printf("Erro: pthread_create -- consumidor");
        }
    }

    for(int i = 0; i < n_threads; i++){
        pthread_join(*(tid+i), NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&cons);
    sem_destroy(&prod);
    free(tid);
    free(buffer);
}

void insere(int e){
    static int in = 0;
    sem_wait(&mutex);
    while(slotsCheios == TAM_BUFFER){
        sem_wait(&prod);
    }
    buffer[in] = e;
    in = (in + 1) % TAM_BUFFER;
    slotsCheios++;
    if(slotsCheios == TAM_BUFFER){
        for(int i = 0; i < n_threads / 2; i++){
            sem_post(&cons);
        }
    }
    sem_post(&mutex);
}

int retira(){
    static int out = 0;
    int e = 0;
    sem_wait(&mutex);
    while(slotsVazios == TAM_BUFFER){
        sem_wait(&cons);
    }

    e = buffer[out];
    out = (out + 1) % TAM_BUFFER;
    slotsCheios--;
    sem_post(&prod);
    sem_post(&mutex);
    return e;
}

void *produtor(){
    for(int i = 0; i < 100; i++){
        insere(i);
    }
    pthread_exit(NULL);
}

void *consumidor(){
    
}
~~~

---

### :hash: Questão 5

A condição para ambas as threads executaram a linha *//SC: usa o recurso* é já existir uma thread daquele tipo usando o recurso, ou ele estar disponível. A primeira thread incrementa o número de threads usando o recurso e pega o sinal que foi disponibilizado na inicialização do semáforo rec, usa o recurso e decrementa o número de threads utilizando o recurso. Quando a última thread terminar de executar, ela devolve o sinal para o semáforo rec.

---

### :hash: Questão 6

- **(a)** O primeiro leitor verifica se tem alguém escrevendo através do semáforo escr, se não houver escritores, ele consome o sinal que estava disponível e libera para que outros leitores possam ler. Se houverem escritores, não haverá um sinal disponível no semáforo escr e o primeiro leitor ficará bloqueado, e por consequência bloqueará os outros leitores visto que não devolverá o sinal do semáforo leit. Sendo assim, o primeiro requisito está sendo atendido. O segundo requisito também está sendo atendido pois, na função *AntesEscrita()* o escritor consome o sinal do semáforo leit impossibilitando que os leitores leiam enquanto está sendo feita uma escrita, e consome o sinal do semáforo escr impossibilitando que um novo escritor tente escrever. O sinal do semáforo escr só é devolvido quando a função *DepoisEscrita()* é chamada, liberando para novos escritores, além disso, os leitores só são liberados quando o último escritor terminar de realizar a operação e o sinal do semáforo leit for devolvido.

- **(b)** Sim, o quarto requisito está sendo atendido pois quando a função *AntesEscrita()* é chamada ela é incrementa o número de escritores, o primeiro escritor bloqueia as operações de leitura, e esta só será desbloqueada quando o número de escritores for igual a zero, ou seja, quando todas as operações de escrita da fila forem realizadas.

- **(c)** Sim, pode ocorrer starvation por parte dos leitores dependendo do número de operações de escrita. Como as operações de escrita são priorizadas, se houverem muitas, os leitores podem permanecer um longo período na espera.

---

### :hash: Questão 7

- **(a)** Se ambas as threads tentarem acessar a seção crítica ao mesmo tempo, aquela que atualizar a variável turno primeiro entrará na seção crítica enquanto a outra thread permanecerá na espera ocupada aguardando a variável *querEntrarX* ser atualizada com o valor necessário para que ela execute a seção crítica.

- **(b)** Ela vai conseguir entrar na seção crítica várias vezes sem sofrer nenhuma uma contenção uma vez que a variável *querEntrarX* necessária para que ela entre na espera ocupada permanecerá igual a zero pois só é preenchida com 1 pela outra thread.

- **(c)** A thread que acabou de sair da seção crítica permanecerá em espera ocupada pois a thread que está na espera ocupada setou a variável *querEntrarX* com valor = 1. Ela só sairá da espera ocupada quando a outra thread terminar de executar.

- **(d)** Sim, o código proposto garante a exclusão mútua no acesso à seção crítica pois como visto acima apenas uma thread consegue executar o trecho de cada vez.

---

### :hash: Questão 8

- **(a)** Quando a thread B recebeu o signal (quando x = 10), não conseguiu pegar o mutex antes da thread A que executou novamente incrementando x para 11, após isso a thread B conseguiu pegar o mutex, não entrou na verificação e printou x = 11.

- **(b)** Na linha 11, deve-se substituir o if por um while, desta maneira a condição será avaliada novamente quando B for desbloqueada mas só conseguir o mutex depois.

---

### :hash: Questão 9

- **(a)** Não. O valor máximo que pode ser impresso é 600, pois com três threads executando 2 vezes a atribuição e soma, o máximo que se pode atingir é 600: 2 * 3 * 100 = 600.

- **(b)** Não, pois pelo menos uma thread vai realizar dois incrementos, logo o valor final será 200.

- **(c)** Sim, trata-se do caso anterior, onde todas as threads executam ao mesmo tempo e apenas os incrementos feitos por uma delas é contabiilizado.

- **(d)** Sim, basta que uma das threads realize um incremento ao mesmo tempo que a outra e essa operação seja perdida.

---

### :hash: Questão 10

- **(a)** Sim, o valor de saldo pode ficar negativo quando uma thread chama a função tarefa com um valor menor que meuSaldo, entra na verificação, mas esta thread perde CPU para uma thread que chamou a função tarefa com um valor maior que meuSaldo, esta atribui o valor à val, perde cpu para a outra thread que estava em execução antes desta, e retira do saldo o valor que foi atribuído pela segunda thread (maior que meuSaldo).

- **(b)** Uma possível correção para este código é implementar uma área de exclusão mútua na função tarefa, desta maneira ficamos com:
~~~c
void *tarefa(void *arg){
    pthread_mutex_lock(&l);
    float val = *(float *) arg;
    float meuSaldo = le();
    if(meuSaldo >= val) retira(val);
    pthread_mutex_unlock(&l);
}
~~~