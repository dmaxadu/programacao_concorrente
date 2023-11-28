#exemplo de uso de futuros em Python

import concurrent.futures
import time
import math as m

x = 10000

#funcao que sera executada de forma assincrona
def ehPrimo(x):
    if(x <= 1): return 0
    if(x == 2): return 1
    if(x % 2 == 0): return 0
    for primes in range(3, int(m.sqrt(x)) + 1, 2):
        if(x % int(primes) == 0): return 0
    return 1

def task(x):
    return ehPrimo(x)

if __name__ == '__main__':
    start = time.time()
    #cria um pool de threads OU de processos com no maximo 5 intancias 
    #with concurrent.futures.ThreadPoolExecutor(max_workers=5) as executor:
    with concurrent.futures.ProcessPoolExecutor(max_workers=5) as executor:
        futures = []
        count = 0
        #submete a tarefa para execucao assincrona
        for aux in range(x):
            futures.append(executor.submit(task, aux))
        #recebe os resultados
        for future in futures:
            result = future.result()
            if(result == 1):
                count += 1
        print(str.format("Quantidade de primos entre 1 e {}: {}", x, count))
    end = time.time()
    print('work took {} seconds'.format(end - start))
