/*
Nome: Andrew da Silva Faria
Disciplina: Programacao concorrente
Professora: Silvana Rossetto
Módulo 2 - Laboratório: 1
Codigo: Usando 8 threads, elevar ao quadrado cada elemento de um vetor de 10000 elementos
*/

//bibliotecas
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <time.h> 

//constantes
#define NTHREADS  8     //total de threads a serem criadas
#define N 10000         //tamanho do vetor que utilizaremos
#define LIMITE 10000    //valor teto para os valores sorteados ao inicializar o vetor

//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct {
   int *vetor, pos, salto, n;
} t_Args;

//funcao que inicializa o vetor de inteiros, escolhendo valores aleatoriamente,
//limitados pelo valor da constante LIMITE.
void initVetor(int *vetor, int n){
    int i;

    srand((unsigned)time(NULL));
    for(i = 0; i < n; i++){
        vetor[i] = rand()%LIMITE;
    }
}

//funcao executada pelas threads
void *elevarQuadrado (void *arg) {
    t_Args *args = (t_Args *) arg;

    for(int i = args->pos; i < args->n; i = i + args->salto){
        args->vetor[i] = args->vetor[i]*args->vetor[i];
    }
    
    free(arg); //libera a alocacao feita na main
    pthread_exit(NULL);
}

// funcao que verifica se o calculo da potencia foi calculado corretamente
// recebe o *vet1 (vetor em que o calculo foi realizado) e *vet2 (vetor original, sem a operacao da potencia)
void verificacao(int *vet1, int *vet2, int n){

    for(int i = 0; i < n; i++){
        if((vet2[i]*vet2[i]) != vet1[i]){
            printf("O calculo esta incorreto!\n");
            return;
        }
    }
    printf("O calculo esta correto!\n");
}

//funcao principal do programa
int main() {
    pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
    t_Args *args;  //receberá os argumentos para a thread

    int vet[N], vetAux[N];

    initVetor(vet, N);

    //copia o vetor 
    for(int i = 0; i < N; i++){
        vetAux[i] = vet[i];
    }

    for(int i=0; i<NTHREADS; i++) {

        args = malloc(sizeof(t_Args));
        if (args == NULL) {
            printf("--ERRO: malloc()\n"); exit(-1);
        }

        args->vetor = vet;
        args->pos = i;
        args->salto = NTHREADS;
        args->n = N;
        
        if (pthread_create(&tid_sistema[i], NULL, elevarQuadrado, (void*) args)) {
            printf("--ERRO: pthread_create()\n"); exit(-1);
        }
    }

    //--espera todas as threads terminarem
    for (int i=0; i<NTHREADS; i++) {
        if (pthread_join(tid_sistema[i], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1); 
        } 
    }

    verificacao(vet, vetAux, N);

    pthread_exit(NULL);
}