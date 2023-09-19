/*
Nome: Andrew da Silva Faria
Disciplina: Programacao concorrente
Professora: Silvana Rossetto
Módulo 2 - Laboratório: 3
Codigo: Usando N threads, controlar a ordem de execução das threads usando variáveis de condição e o padrão barreira.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* Variaveis globais*/
int count = 0;      //conta quantas threads ja chegaram na barreira
int nthreads = 0;   // variavel que guardara o numero de threads passado como argumento na linha de comando
pthread_mutex_t mutex;
pthread_cond_t cond;

/* Funcao de barreira */
void barreira()
{
    pthread_mutex_lock(&mutex);
    count+= 1;
    if(count < nthreads)
    {
        pthread_cond_wait(&cond, &mutex);
    }
    else
    {
        printf("\n");
        count = 0;
        pthread_cond_broadcast(&cond);
    }
    pthread_mutex_unlock(&mutex);
}

// funcao que cada thread realizará, printando o que foi pedido
void *Prints(void *arg)
{
    long int id = (long int) arg;

    printf("Olá da thread %i\n", (int) id + 1);
    barreira();

    printf("Que dia bonito %i\n", (int) id + 1);
    barreira();

    printf("Até breve da thread %i\n", (int) id + 1);

    pthread_exit(NULL);
}

// funcao main 
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        return 1;
    }

    nthreads = atoi(argv[1]);
    pthread_t tid[nthreads];

    pthread_mutex_init(&mutex, NULL);

    for(long int i = 0; i < nthreads; i++)
    {
        //criando as threads
        if(pthread_create(&tid[i], NULL, Prints, (void*) i)) 
        {
            printf("--ERRO: pthread_create()\n");
            exit(-1);
        }
    }

    for(int i = 0; i < nthreads; i++)
    {
        // realiza o join das threads
        if(pthread_join(tid[i], NULL))
        {
            printf("--ERRO: pthread_join()\n");
            exit(-1);
        }
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}