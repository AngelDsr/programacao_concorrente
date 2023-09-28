#include <stdio.h>
#include <pthread.h>

// IMPLEMENTANDO PADRAO LEITOR / ESCRITOR

void initLeit(int *escr, int *leit, pthread_mutex_t *mutex, pthread_cond_t *cond_leit);

void initEscr(int *escr, int *leit, pthread_mutex_t *mutex, pthread_cond_t *cond_escr);

void saiLeit(int *leit, pthread_mutex_t *mutex, pthread_cond_t *cond_escr);

void saiEscr(int *escr, pthread_mutex_t *mutex, pthread_cond_t *cond_leit, pthread_cond_t *cond_escr);