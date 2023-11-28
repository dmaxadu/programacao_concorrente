# :zap: Laboratório 2
### :hash: Atividade 1:
- **Os valores impressos foram sempre o valor esperado? Se não, explique por que isso aconteceu?**
    **R:** Não, pois como a variável *soma* é global e compartilhada pelas threads, ambas vão ler o mesmo valor ao mesmo tempo e incrementar a variável, no entanto ao escrever o valor nesta novamente, ambas as threads podem devolver o mesmo valor para a variável sobreescrevendo uma à outra.

- **É possível que a variável **soma** termine com valor acima de 200.000? Por que?**
    **R:** Não é possível pois, mesmo que as duas threads não executem a incrementação ao mesmo tempo, o máximo que podemos obter é 200.000 pois é uma soma de 100.000 de cada thread.

---
### :hash: Atividade 2
- **Os valores impressos foram sempre o valor esperado? Por que?**
    **R:** Sim, pois agora estamos usando as funções pthread_mutex_lock e pthread_mutex_unlock para definir uma seção crítica onde se dois fluxos forem executados ao mesmo tempo, podemos ter problema. Essa função bloqueia um fluxo enquanto outro está executando. Com isso, conseguimos garantir que os fluxos vão ser executados um de cada vez e o resultado esperado vai ser obtido, neste caso cada thread vai retornar 100.000 e o valor da variável soma será 200.000.
