# Resultados e Análise do Lab4

## Observação:
 - Para executar o programa, utilize a seguinte linha de comando
 
    gcc -o nomeExecutavel list_int.c my_rw.c main_lab4.c

## Programa utilizando lock:
Número de threads - Tempo médio de execução
1 - 0.49
2 - 1.51
3 - 1.3
4 - 1.65

## Programa utilizando rwlock:
Número de threads - Tempo médio de execução
1 - 0.51
2 - 4.15
3 - 2.96
4 - 9.42

## Programa utilizando padrão leitor/escritor:
Número de threads - Tempo médio de execução
1 - 0.62
2 - 1.47
3 - 2.53
4 - 10.42

## Análise

O objetivo do laboratório foi trabalhar o padrão leitores/escritores. Esse padrão permite que diferentes threads possam realizar leituras de forma simultânea, visto que o acesso de uma variável compartilhada não estará sendo alterado. Porém a operação de escrita só pode ser realizada individualmente, visto que ela causará alteração na variável global.

Assim, após executar os programas solicitados, os resultados mostrados foram estranhos (diferentes do esperado). Acredito que isso aconteceu por algum aspecto ou configuração da minha máquina, pois no laboratório da universidade os resultados estavam diferentes dos resultados que consegui na minha máquina. C

Ao olhar as execuções com 1 única thread, notamos que os resultados são muito semelhantes. Isso é esperado, visto que no caso de 1 thread, o programa fica muito próximo de um programa sequencial, não tendo nenhum ganho de performance com a programação concorrente.

No caso de 2 threads, vemos que o programa que utiliza rwlock teve um aumento expressivo em seu tempo de execução, algo que foi estranho de acontecer. Quando executei o programa no laboratório, o resultado foi bem diferente. O tempo de execução estava levemente melhor em relação ao programa que utiliza apenas o lock. Já o programa com o padrão leitor/escritor escrito por mim foi ligeiramente melhor que o lock, algo que é razoável de acontecer, visto que com 2 threads começamos a usufruir da possibilidade de fazer múltiplas leituras simultâneas.

Para a execução com 3 threads, observamos que os programas com rwlock e aquele implementado por mim tiveram um resultado parecido, mas ambos ficaram com desempenho razoavelmente pior que o outro, o que, novamente, é ocorrência inesperada.

Por fim, utilizando 4 threads, temos um resultado semelhante ao de 3 threads, porém amplificado. O tempo de execução do programa com lock continua baixo, enquanto os demais aumentaram consideravelemente.

Teoricamente, acho que a medida que aumentassemos o número de threads, os programas 2 (rwlock) e 3 (implementado por mim) deveriam passar a ter um desempenho melhor que o programa 1 (que utiliza apenas lock) por utilizarem o padrão leitor/escritor.