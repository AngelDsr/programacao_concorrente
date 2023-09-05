/*
Nome: Andrew da Silva Faria
Disciplina: Programacao concorrente
Professora: Silvana Rossetto
Módulo 3 - Laboratório: 2
Codigo: Dada uma sequencia consecutiva de numeros naturais (inteiros positivos) de 1 a N (N muito grande), identificar todos os numeros primos dessa sequencia e retornar a quantidade total de numeros primos encontrados
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"
#include <math.h>

void * contaPrimos(void *arg);
int ehPrimo (long long int n );

int nthreads; //numero de threads
int soma_total; //quantidade total de primos na sequencia
pthread_mutex_t mutex;

typedef struct{
   int id; //identificador do elemento que a thread ira processar
   long long int tamanho; //dimensao das estruturas de entrada
   long long int * vetor;
} tArgs;

//funcao que as threads executarao
void * contaPrimos(void *arg) {
    int soma_local = 0;
    tArgs *args = (tArgs*) arg;
    //printf("Thread %d\n", args->id);

    for(int i=args->id; i<args->tamanho; i+=nthreads){
        if(ehPrimo(args->vetor[i])){
            soma_local++;
        }
    }
    pthread_mutex_lock(&mutex);
     //--SC
    soma_total += soma_local;
     //--saida da SC
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int ehPrimo (long long int n ){
    int i;
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (i = 3; i < sqrt((double)n)+1; i+= 2)
        if (n%i==0) return 0;
    return 1;
}

//fluxo principal
int main(int argc, char* argv[]) {
    long long int tamanho; //dimensao da matriz de entrada
    pthread_t *tid; //identificadores das threads no sistema
    tArgs *args; //identificadores locais das threads e dimensao
    long long int * vetor;
    double inicio, fim, delta;

    //--inicilaiza o mutex (lock de exclusao mutua)
    pthread_mutex_init(&mutex, NULL);
   
    /////////GET_TIME(inicio);
    //leitura e avaliacao dos parametros de entrada
    if(argc<3) {
        printf("Digite: %s <tamanho da sequencia> <numero de threads>\n", argv[0]);
        return 1;
    }
    tamanho = atoll(argv[1]);
    nthreads = atoi(argv[2]);
    if (nthreads > tamanho) nthreads=tamanho;

    //alocacao de memoria para a estrutura de dados
    vetor = (long long int *) malloc(sizeof(long long int) * tamanho);
    if (vetor == NULL) {printf("ERRO--malloc\n"); return 2;}

    //inicializacao do vetor
    for(int i=0; i<tamanho; i++) {
        vetor[i] = i + 1;
    }

    //contagem de primos da sequencia
    GET_TIME(inicio);

    //alocacao das estruturas
    tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
    if(tid==NULL) {puts("ERRO--malloc"); return 2;}
    args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
    if(args==NULL) {puts("ERRO--malloc"); return 2;}

    //criacao das threads
    for(int i=0; i<nthreads; i++) {
        (args+i)->id = i;
        (args+i)->tamanho = tamanho;
        (args+i)->vetor = vetor;
        if(pthread_create(tid+i, NULL, contaPrimos, (void*) (args+i))){
            puts("ERRO--pthread_create"); return 3;
        }
    } 
    //espera pelo termino da threads
    for(int i=0; i<nthreads; i++) {
        pthread_join(*(tid+i), NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("A quantidade de numeros primos eh: %i\n", soma_total);

    GET_TIME(fim)   
    delta = fim - inicio;
    printf("Tempo procesamento sequencia (tamanho %lli) (nthreads %d): %lf\n", tamanho, nthreads, delta);

    //liberacao da memoria
    free(vetor);
    free(args);
    free(tid);

    return 0;
}

/*
RESULTADOS TESTES:
N -> tamanho do vetor
T -> numero de threads

N = 1000; T = 1
Tempo medio = 0.0002612 s

N = 1000000; T = 1
Tempo medio = 0.161306 s


N = 1000; T = 2
Tempo medio =  0.000273 s
Aceleracao = 0.0002612 / 0.000273 = 0,95677655677655677655677655677656
Eficiencia = 0,95677655677655677655677655677656 / 2 = 0,47838827838827838827838827838828

N = 1000000; T = 2
Tempo medio = 0.160778 s
Aceleracao = 0.161306 / 0.160778 = 1,0032840313973304805383821169563
Eficiencia = 1,0032840313973304805383821169563 / 2 = 0,501642015


N = 1000; T = 4
Tempo medio = 0.0003794 s
Aceleracao = 0.0002612 / 0.0003794 = 0,68845545598313125988402741170269
Eficiencia = 0,68845545598313125988402741170269 / 4 = 0,17211386399578281497100685292567

N = 1000000; T = 4
Tempo medio = 0.0812032 s
Aceleracao = 0.161306 / 0.0812032 = 1,9864488098991172761664564943253
Eficiencia = 1,9864488098991172761664564943253 / 4 = 0,49661220247477931904161412358133


ANÁLISE:
Com os resultados obtidos, é possível perceber que houveram alguns que, num primeiro momento, se mostram incoerentes com o objetivo da programação concorrente. Uma das motivações da programação concorrente é tentar melhorar o desempenho das aplicações, e estamos dividindo a execução do programa em diferentes fluxos de execução (thread) para atingir tal objetivo. Porém, vemos que só há uma melhoria no desempenho quando N = 10^6 e T = 4. Nos demais casos, o desempenho sequencial (utilizando apenas 1 thread) é melhor, ou levemente pior que os demais.
Analisando quando o tamanho da entrada é menor (N = 10^3), observamos que a utilização de apenas uma única thread é levemente mais vantajosa. Por mais que isso possa ser uma surpresa, é algo crível de se acontecer. Isso porque justamente pela entrada ser menor. Ao contruir um programa concorrente estaremos buscando resolver problemas com entradas muito grandes, tirando real proveito da concorrência. Assim, quando temos uma entrada pequena, pode ser que as etapas que buscam garantir a concorrência do programa (criação e divisão da tarefa em threads, utilização do lock se houver, ... ) tenham um peso no tempo de execução do programa muito grande em relação ao tempo sequencial.
Agora, olhando para os resultados de quando N = 10^6 e T = 2, percebe-se que houve uma melhoria extremamente baixa em relação ao tempo sequencial obtido. Diferente do caso mencionado acima, a entrada neste caso já possui um valor considerável, no qual já deveríamos ver uma melhoria maior. Então, por que o resultado está assim? O problema está no fato de que ao dividirmos a tarefa de contar os números primos para cada thread, não estamos necessariamente dividindo igualmente a carga de trabalho. O modo escolhido de dividir a tarefa foi simplesmente alternar os numeros analisados entre as threads. Desse modo, tendo apenas 2 threads, essa divisão fez com que a thread 1 irá checar os números ímpares e a thread 2 os pares. Porém, observando a função ehPrimo(long long int n ), podemos concluir que a thread 2, que sempre estará analisando valores pares, irá sempre terminar sua execução muito rápido, sem nem precisar passar pelo loop da função, enquanto a thread 1 irá ter muito mais trabalho tendo que passar pelo loop várias vezes, e por um tempo cada vez maior a medida que os números ímpares ficam cada vez maiores. Diante disso, vemos um desbalanceamento da carga de trabalho entre as threads, o que justifica a pequena diferença de tempo entre o programa concorrente e o sequencial.
Se observarmos quando N = 10^6 e T = 4, ja vemos uma melhoria considerável, pois a carga de trabalho foi distribuida melhor entre as threads, mas eu particularmente acho que se utilizarmos a divisão proposta em sala (em que temos uma variável global indicando o índice do próximo valor a ser checado se é primo) teríamos uma melhora ainda maior. Isso porque da maneira que está implementado acima, digamos que as threads fiquem divididas da seguinte forma:
thread 1 = 1 + 4*i ; thread 2 = 2 + 4*i ; thread 1 = 3 + 4*i ; thread 1 = 4 + 4*i , é possível perceber que as threads 1 e 3 ficariam apenas com valores ímpares e as threads 2 e 4 com os pares. Assim, pode-se dizer que apesar de estar melhor dividido, a carga de trabalho de 1 e 3 será maior que das threads 2 e 4.



*/