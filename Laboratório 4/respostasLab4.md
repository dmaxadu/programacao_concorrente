# :zap: Laboratório 4

---

### :hash: Atividade 1
- **O TAD lista implementado nesses arquivos poderia ser compartilhado por threads de um programa concorrente? Com qual finalidade e de que forma?**
    R: Sim, poderia ser implementado em um programa concorrente utilizando o padrão escritores/leitores.

- **O que poderia acontecer se o programa não implementasse exclusão mútua no acesso às operações da lista encadeada?**
    R: Uma thread poderia por exemplo tentar ler um valor que não existe mais na lista, pois foi excluído por uma outra thread e isso acarretaria em um segmentation fault.

- **O que acontece com o tempo de execução do programa quando aumentamos o número de threads? Por que isso ocorre?**
    R: A maior diferença no tempo de execução ocorre quando usamos n > 1 threads. Isso ocorre pois utilizando apenas uma thread, apesar de gastar o tempo chamando a função lock, ela nunca se bloqueia de fato pois o lock está sempre à sua disposição. Quando aumentamos a quantidade de threads, em determinados momentos haverá um conflito devido ao lock estar com uma thread enquanto outra tenta utilizá-lo e isso acaba gerando um gasto maior de tempo.

---

### :hash: Atividade 2
- **Em quais cenários o uso do rwlock pode ser mais vantajoso que o uso do lock de exclusão mútua?**
    R: Será mais vantajoso em casos onde existem mais operações de leitura do que escrita, pois diminuirá o gasto de tempo que ocorre quando o há o conflito em decorrência do lock.

---
### :hash: Atividade 3
- **Análise comparativa dos tempos de execução das versões com mutex e rwlock e a minha implementação do padrão leitores/escritores:**
    - ###### Utilizando exclusão mútua (tempo em segundos):
    _Compilação: gcc -o teste1 main_list_exc_mut.c list_int.c -lpthread -Wall_

    Número de Threads   | Execução 1 | Execução 2 | Execução 3 | Execução 4
    :-----------------: | :--------: | :--------: | :--------: | :----------: 
    1                   | 1.552137   | 1.559393   | 1.555710   | 1.562143
    2                   | 4.710115   | 4.436713   | 4.478013   | 4.005227
    
    - ###### Utilizando rwlock da biblioteca pthread (tempo em segundos):
    _Compilação: gcc -o teste2 main_list_rwlock.c list_int.c -lpthread -Wall_

    Número de Threads   | Execução 1 | Execução 2 | Execução 3 | Execução 4
    :-----------------: | :--------: | :--------: | :--------: | :----------: 
    1                   | 1.546516   | 1.549802   | 1.546083   | 1.574291
    2                   | 2.946164   | 2.510820   | 2.919807   | 2.982398

    - ###### Utilizando a biblioteca implementada por mim (tempo em segundos):
    _Compilação: gcc -o teste3 main_list_leit_escr.c list_int.c leitores_escritores.c  -lpthread -Wall_
    
    Número de Threads   | Execução 1 | Execução 2 | Execução 3 | Execução 4
    :-----------------: | :--------: | :--------: | :--------: | :----------: 
    1                   | 1.793951   | 1.786923   | 1.793899   | 1.811221
    2                   | 3.720969   | 3.489066   | 3.555388   | 3.707777

    De posse dos resultados podemos perceber que houve uma melhora significativa do tempo de execução do programa para 2 threads utilizando a minha biblioteca em relação ao programa utilizando a exclusão mútua. Já era esperado que a implementação utilizando o rwlock da biblioteca pthread obtivesse um desempenho melhor do que a implementação utilizando a minha biblioteca, no entanto comparando os tempos podemos perceber que a diferença não é tão gritante.