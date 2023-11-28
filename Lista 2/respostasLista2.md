# :zap: Lista 2 - Programação Concorrente
Daniel Machado da Conceição - 121046088

---

### :hash: Questão 1

- **(a) O que é seção crítica do código em um programa 
concorrente?**

    É um trecho de código que pode ser acessado simultâneamente por mais de uma thread e pelo menos uma das operações realizadas é de escrita.

- **(b) O que é condição de corrida em um programa concorrente?**

    A condição de corrida é determinada pela possibilidade de gerar um resultado diferente do esperado dependendo da ordem de execução das threads.

- **(c) Como funciona a sincronização por exclusão mútua?**

    A sincronização por exclusão mútua funciona através da implementação de um mecanismo chamado mutex que possibilita que apenas uma thread (a que estiver de posse do mutex) execute o trecho compreendido nela.

- **(d) Por que os mecanismos de comunicação e sincronização são necessários para a programação concorrente?**

    Os mecanismos de comunicação são necessários para que haja comunicação entre as threads e desta maneira, um compartilhamento de dados, informações de processamento e estado da aplicação. Além disso, a comunicação é fundamental para que haja sincronização entre as threads. Esta é necessária para garantir que o acesso aos recursos compartilhados ocorram corretamene e para que se produza o resultado esperado, no tempo esperado e na ordem requerida.

- **(e) Como funciona a sincronização condicional (que usa as funções _wait, signal e broadcast_)?**
    
    A sincronização condicional funciona através da implementação de variáveis condicionais utilizadas para bloquear a execução de um fluxo enquanto aquela condição não for satisfeita. A função wait é responsável por realizar o bloqueio da thread se a condição não for satisfeita. A função signal é chamada pela thread responsável por avaliar se a condição foi satisfeita ou não e libera apenas uma thread que está na fila de bloqueio, enquanto a função broadcast libera todas as threads que estão na fila.

****

### :hash: Questão 2

Logo de início podemos ver que os valores 4 e -4 não podem ser exibidos nunca pois não existem incrementos e decrementos no código suficientes para atingir esse valor. Para os outros valores, vamos fazer uma análise separada para cada um:

- O valor 1 pode ser exibido quando a thread 3 executa completamente antes de perder CPU para outra thread;

- O valor -1 pode ser exibido quando a thread 1 executa completamente antes de perder CPU para outra thread;

- O valor 0 pode ser exibido quando: a thread 1 executa até o if, perde CPU para a thread 2 que incrementa x = -1 ficando com x = 0, perde CPU para a thread 1 e como já foi feita a verificação anteriormente antes da execução da thread 2, printa o valor de x = 0;

- O valor 2 pode ser exibido quando a thread 3 executa até o if, perde CPU para a thread 2 que incrementa o valor x = 1 para x = 2, perde CPU para a thread 3 e como ja foi feita a verificação anteriormente antes da execução da thread 2, printa o xalor de x = 2;

- O valor -2 pode ser exibido quando a thread 3 executa até o if, a thread 1 e a thread 2 executam a linha 1 ao mesmo tempo mas a thread 1 sobreescreve a thread 2, em seguida a thread 1 e a thread 2 executam a linha 2 ao mesmo tempo mas a thread 2 sobreescreve a thread 1, a thread 1 executa a linha 3 e a thread 3 imprime o valor de x = -2;

- O valor 3 pode ser exibido quando a thread 3 executa até o if, a thread 1 e a thread 2 executam simultâneamente a primeira linha, mas a thread 2 sobreescreve a thread 1, a thread 1 executa a segunda linha e volta para a thread 3 após o if printando o valor x = 3.

- O valor -3 pode ser exibido quando a thread 1 e a thread 2 lêem x = 0 mas não executam nada, a thread 3 executa até o if, a thread 1 e a thread 2 executam a linha 1 simultâneamente mas a thread 1 sobreescreve a thread 2, a thread 1 e a thread 2 executam a linha 2 simultâneamente mas a thread 2 sobreescreve a thread 1, a thread 1 executa a linha 3 e a thread 3 imprime o valor x = -3.

Desta maneira, concluímos que apenas os valores -3, -2, -1, 0, 1, 2 e 3 podem ser impressos.

****

### :hash: Questão 3
#### **Caso 1**
- **(a)** A ordem de execução das sentenças que vai gerar o erro é: S1 -> S3 -> S2.
- **(b)** Uma possível correção para este trecho de código é adicionar uma área de exclusão mútua em amabas as threads para permitir que apenas um fluxo seja executado por vez:

~~~c
 pthread_mutex_t mutex

 T1:
 pthread_mutex_lock(&mutex);
 S1: if(thd->proc_info){
    S2: fputs(thad->proc_info, ...);
 }
 pthread_mutex_unlock(&mutex);

 T2:
 pthread_mutex_lock(&mutex);
 S3: thd->proc_info = NULL;
 ...
 pthread_mutex_unlock(&mutex);
~~~
#### **Caso 2**
- **(c)** Uma possível correção para este erro seria utilizar o mutex para implementar a exclusão mútua, uma variável de controle para sinalizar quando a condição foi satisfeita e uma variável de condição para realizar o bloqueio de thread enquanto a condição não for satisfeita:

~~~c
pthread_mutex_t mutex;
pthred_cont_t cond;
int controle = 0;

T1:
pthread_mutex_lock(&mutex);
void init(...){
    mThread = PR_CreateThread(mMain, ...);
}
controle = 1;
pthread_signal(&cond);
pthread_mutex_unlock(&mutext);

T2:
void mMain(...){
    pthread_mutex_lock(&mutex);
    if(controle == 0){
        pthread_wait(&cond, &mutex);
    }
    mState = mThread->State;
    pthread_mutex_unlock(&mutex);
}
~~~

---

### :hash: Questão 4

- **(a)** Este código respeita todos os requisitos lógicos do padrão leitores/escritores:
    
    - Quando alguém está escrevendo, ninguém pode ler ou escrever. Isso é garantido pela função _InicEscr()_ que verifica se existem leitores ou escritores e em caso positivo, bloqueia a thread;

    - Quando alguém está lendo, é permitida a leitura por outras threads, mas ninguém pode escrever. Esse comportamento é garantido pela função _InicLeit()_ que verifica apenas se há alguém escrevendo (possibilitando outros leitores) e em caso positivo bloqueia a thread.

- **(b)** Os blocos while não podem ser substituídos por if, pois no momento que a thread é desbloqueada ela retorna no mesmo ponto onde se bloqueou (dentro do bloco), se o while for substituído por if a verificação não vai ser feita novamente e desta maneira as condições lógicas do padrão não serão respeitadas.

---

### :hash: Questão 5
- **(a)** Sim atende ao requisitos colocados pois a thread 3 só executará quando a variável de controle _chegada_ for igual a 2, e isso só ocorre quando as threads 1 e 2 já executaram. Uma vez que a thread 3 executou, ela incrementa a variável _sentados_ que é utilizada para liberar a execução das threads 4 e 5.

- **(b)** Nessa programa em específico, os blocos de while poderiam ser substituídos por if pois uma vez que as threads 1 e 2 já executaram, a variável de controle _chegadas_ nunca vai ser alterada depois disso, o mesmo ocorre para a variável de controle _sentados_ após a execução da thread 3. Portanto, como não há a necessidade de refazer a verificação no momento que as threads são desbloqueadas, os blocos de while podem ser substituídos por if.

---

### :hash: Questão 6
- **(a)**
~~~c
double calculaPi(n){
    double pi = 0;
    for(int i = 0; i < n; i++){
        pi += ((4/(8*i+1))-(2/(8*i+4))-(1/(8*5+5))-(1/(8*i+6)))*(1/(16**i))
    }
    return pi;
}
~~~
- **(c)**
~~~c
//variáveis globais
int n; //assumindo que n seja passado na linha de comando;
double piGlobal = 0;
pthread_mutex_t mutex;
void calculaPiConcorrente(void *arg){
    int id_thread = *(int *) arg;
    double piLocal = 0;

    for(int i = id_thread; i < n; i += m_threads){
        piLocal += ((4/(8*i+1))-(2/(8*i+4))-(1/(8*5+5))-(1/(8*i+6)))*(1/(16**i));
    }

    pthread_mutex_lock(&mutex);
    piGlobal += piLocal;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}
~~~

---

### :hash: Questão 7

- **(a)** Esse problema pode se beneficiar de uma solução concorrente através da divisão das requisições entre várias threads. Sendo assim, teríamos *n_threads* para as operações de leitura e processamento e se for necessário escrever algum dado, entra uma área de exclusão mútua.

- **(b)**
~~~c
pthread_mutex_t mutex;
int precisaEscrever = 0;

void processaRequisicoes(){
    pthread_mutex_lock(&mutex);
    req = requisicoes.next();
    pthread_mutex_unlock(&mutex);
    n = requisicoes.length();

    for(int i = 0; i < n; i += n_threads){
        //faz leitura
        //processa dados
        // verifica se precisa escrever, se sim
            precisaEscrever = 1;
    }

    if(precisaEscrever > 0){
        pthread_mutex_lock(&mutex);
        //escreve
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}
~~~
