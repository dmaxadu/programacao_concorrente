# :zap: Laboratório 5
---

### :hash: Atividade 1
- **Os valores impressos foram sempre o valor esperado? Por que?**
    R: Sim. Porque o semáforo, responsável por implementar uma área de exclusão mútua, permitiu que as threads executassem de maneira correta, sem sobreescrever o resultado uma da outra e sem que as duas decrementassem ao mesmo tempo. Aumentado o número de sinais iniciais para 2 por exemplo, o valor impresso não condiz com o valor esperado pois ambas as threads em algum momento decrementaram **s** ao mesmo tempo.

---
### :hash: Atividade 2
- **Quais são os possíveis valores finais da variável x?**
    R: Só existe um valor possível para x, que é x = 2. Isso corre pois a thread T2 que espera a execução de T1 acabar para iniciar, sempre atribui o valor 2 para a variável x. A thread T3 responsável por imprimir o valor de x, só executa quando T2 finaliza e já atribuiu 2 à x.

- **O valor final da variável x variou? Por que?**
    R: Não pois a thread 3, responsável por imprimir o valor de x, só executa quando a thread 2 já executou, e esta atribui a x o valor 2.

- **Altere o valor de inicialização dos semáforos de 0 para 1, execute o programa várias vezes e observe os resultados impressos na tela. O que aconteceu e por que?**
    R: Os resultados variam, isso ocorre pois quando as threads chegam ao semáforo e encontram um sinal, a ordem de execução pode ser alterada já que elas não vão se bloquear. Desta maneira, temos como possíveis resultados para x os valores:
    - 0 se T3 executar primeiro que as outras threads;
    - 1 se T2 executar antes de T1, pois dessa maneira o último valor atribuido a x é 1;
    - 2 se a ordem de execução for respeitada e T2 executar depois de T1.

---
### :hash: Atividade 3

- **Qual é a finalidade das variáveis in e out e por que foram definidas como variáveis static?**
    R: As variáveis in e out são responsáveis por determinar o índice do buffer para inserção ou retirada de elementos reespectivamente. Isso é feito através do incremento de ambas em função do módulo de N, quando a posição coincidir com o valor de N, a variável será zerada garantido que aponte para a posição inicial do buffer. Dessa forma garantimos por exemplo que um produtor não vai inserir algo em um slot que já está preenchido ou que um consumidor não vai tentar consumir algo de um slot que está vazio. Ambas foram declaradas como static para que seus valores sejam salvos ao longo da execução do programa, como se fossem variáveis globais mas dentro do escopo da função. Isso é importante para que não se perca o acompanhamento dos índices do buffer e sejam feitas operações de inserção ou remoção no índice errado.

- **O programa executou corretamente? Como isso foi demonstrado?**
    R: Sim, até onde analisei, o programa executou corretamente. Posso garantir isso, pois as condições do padrão produtor/consumidor foram respeitadas:
    - Um produtor só inseriu um dado quando existiam slots vazios, e não inseriu em um slot que já estava preenchido;
    - Um consumidor só consumiu um dado quando o buffer não estava vazio, e não tentou consumir de um slot que estava vazio.
    
    Esse comportamento foi observado após analisar o log produzido pelo programa.