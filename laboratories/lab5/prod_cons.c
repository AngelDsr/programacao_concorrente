/*
Nome: Andrew da Silva Faria
Disciplina: Programacao concorrente
Professora: Silvana Rossetto
Módulo 5 - Laboratório: 5
Codigo: Programa concorrente onde a thread main (PRODUTORA) cria N threads (CONSUMIDORAS), depois le linha a linha o conteudo de um arquivo texto e as deposita em um buffer compartilhado. As threads devem retirar as linhas do buffer e imprimi-las na saida padrao.
*/

//bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// definindo constantes
#define LENGTH 3

// prototipos das funcoes
void le_arquivo(char *line, FILE *file);
void *consumidor(void *arg);
void Insere(char *l);
void Retira();

//variaveis globais
char *buffer[LENGTH];
int idx = 0;
int nthreads;

// variaveis semaforos
sem_t mutexGeral;
sem_t slotCheio, slotVazio;

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("Error.\n");
        return 1;
    }

    nthreads = atoi(argv[1]);
    pthread_t tid[nthreads];

    // Inicializando semaforos
    sem_init(&mutexGeral, 0, 1);
    sem_init(&slotCheio, 0, 0);
    sem_init(&slotVazio, 0, LENGTH);

    // Criando threads
    for(long int i = 0; i < nthreads; i++)
    {
        if(pthread_create(&tid[i], NULL, consumidor, (void *) i))
        {
            printf("--ERRO: pthread_create()\n");
            exit(-1);
        }
    }

    // abrindo arquivo para leitura
    char *filename = argv[2];
    FILE *file = fopen(filename, "r");
    if(file == NULL)
    {
        printf("Error: file pointer is null.\n");
        exit(1);
    }

    char *line = NULL;
    le_arquivo(line, file);

    // espera todas as threads terminarem
    for(int i = 0; i < nthreads; i++)
    {
        if (pthread_join(tid[i], NULL))
        {
            printf("--ERRO: pthread_join()\n");
            exit(-1);
        }
    }

    pthread_exit(NULL);
    fclose(file);

    for(int i = 0; i < LENGTH; i++)
    {
        free(buffer[i]);
    }
    if(line)
    {
        free(line);
    }

    return 0;
}

//funcao que realiza leitura do arquivo e escreve no buffer (em outras palavras, eh a funcao do produtor)
void le_arquivo(char *line, FILE *file)
{
    // variaveis para realizar a copia das informacoes do texto de entrada
    ssize_t line_length;
    size_t len = 0;

    // produtor: le cada linha do texto, e escreve no buffer
    while(1)
    {
        line_length = getline(&line, &len, file);
        if(line_length == -1)
        {
            break;
        }

        char *tmp = malloc(sizeof(char)*line_length);
        if(tmp == NULL)
        {
            printf("Error: couldn't allocate memory.\n");
            return;
        }

        strcpy(tmp, line);
        Insere(tmp);

        // Gerea delay
        sleep(1);
    }
}

// Consumidor 
void *consumidor(void *arg)
{
    while(1){
        Retira();

        // Gera delay
        sleep(2);
    }
    pthread_exit(NULL);
}

// funcao que insere no buffer a informacao desejada
void Insere(char *l)
{
    static int in = 0;
    // verifica o semaforo para ver se ha slots vazios para poder escrever no buffer
    sem_wait(&slotVazio);
    
    // exclusao mutua usando semaforo para alterar variavel global `buffer`
    sem_wait(&mutexGeral);
    buffer[in] = l;
    in = (in + 1) % LENGTH;
    sem_post(&mutexGeral);

    // libera um sinal indicando que ha mais um slot cheio no buffer
    sem_post(&slotCheio);
}

// funcao que retira do buffer a informacao desejada
void Retira()
{
    static int out = 0;

    // verifica o semaforo para ver se ha slots vazios
    sem_wait(&slotCheio);

    // exclusao mutua usando semaforo
    sem_wait(&mutexGeral);
    printf("%s", buffer[out]);
    buffer[out] = NULL;
    out = (out + 1) % LENGTH;
    sem_post(&mutexGeral);

    // libera um sinal indicando que ha mais um slot vazio
    sem_post(&slotVazio);
}
