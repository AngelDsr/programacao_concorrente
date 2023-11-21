# Aluno: Andrew da Silva Faria
# Disciplina: Programacao Concorrente
# Prof.: Silvana Rossetto 
# Laboratório: 9
# Codigo: Criando um pool de threads em Python


from multiprocessing.pool import  Pool
import time
from math import sqrt
import sys

#funcao que sera executada de forma concorrente
def ehPrimo(n):
    if (n <= 1):
        return 0
    if (n == 2): 
        return 1
    if (n % 2 == 0):
        return 0

    i = 3
    while i < sqrt(n) + 1:
        if(n % i == 0): 
            return 0
        i +=2
    return 1

# Funcao contadora de primos sequencial para verificar corretude 
def ehPrimoSeq(n):
    contador = 0
    for i in range(1, n+1):
        if ehPrimo(i):
            contador += 1
    return contador

# Funcao main
if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Usage: python primos.py tamanhoDaSequencia")
        exit(1)
    if not sys.argv[1].isdigit():
        print("Usage: python primos.py tamanhoDaSequencia(NUMERO)")
        exit(1)

    N = int(sys.argv[1])
    start = time.time()
    #por default, cria um processo distinto para cada processador da maquina
    pool = Pool()

    numbers = list(range(1, N)) 

    results = pool.map(ehPrimo,numbers)
    primos = sum(results)
    print(f"Numero de primos usando pool de threads: {primos}")
    end = time.time()

    #descomente para verificar corretude (vendo o resultado usando um algoritmo sequencial para contar primos)
    #print(f"Numero de primos sequencial: {ehPrimoSeq(N)}")

    print('work took {} seconds'.format(end - start))
