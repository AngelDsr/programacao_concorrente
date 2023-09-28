#include <stdio.h>
#include <pthread.h>
#include "my_rw.h"

// IMPLEMENTANDO PADRAO LEITOR / ESCRITOR

// Funcao que inicializa leitura.
void initLeit(int *escr, int *leit, pthread_mutex_t *mutex, pthread_cond_t *cond_leit)
{
    pthread_mutex_lock(mutex);
    while(*escr > 0)
    {
        pthread_cond_wait(cond_leit, mutex);
    }
    (*leit)++;
    pthread_mutex_unlock(mutex);
}

// Funcao que inicializa escrita.
void initEscr(int *escr, int *leit, pthread_mutex_t *mutex, pthread_cond_t *cond_escr)
{
    pthread_mutex_lock(mutex);
    while ((*escr > 0) || (*leit > 0))
    {
        pthread_cond_wait(cond_escr, mutex);
    }
    (*escr)++;
    pthread_mutex_unlock(mutex);
}

// Funcao que finaliza leitura.
void saiLeit(int *leit, pthread_mutex_t *mutex, pthread_cond_t *cond_escr)
{
    pthread_mutex_lock(mutex);
    (*leit)--;
    if(*leit == 0)
    {
        pthread_cond_signal(cond_escr);
    }
    pthread_mutex_unlock(mutex);
}

// Funcao que finaliza leitura.
void saiEscr(int *escr, pthread_mutex_t *mutex, pthread_cond_t *cond_leit, pthread_cond_t *cond_escr)
{
    pthread_mutex_lock(mutex);
    (*escr)--;
    pthread_cond_broadcast(cond_leit);
    pthread_cond_signal(cond_escr);
    pthread_mutex_unlock(mutex);
}

