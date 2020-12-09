#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#define N_HIDROGENIO 2  // Define o total de atomos de hidrogenio a serem gerados.
#define N_ENXOFRE 1     // Define o total de atomos de enxofre a serem gerados.
#define N_OXIGENIO 4    // Define o total de atomos de oxigenio a serem gerados.

// Cria e inicializa os semaforos para cada thread.
static pthread_mutex_t mutex_h = PTHREAD_MUTEX_INITIALIZER;  // Semaforo para thr_Hidrogenio.
static pthread_mutex_t mutex_s = PTHREAD_MUTEX_INITIALIZER;  // Semaforo para thr_Enxofre.
static pthread_mutex_t mutex_o = PTHREAD_MUTEX_INITIALIZER;  // Semaforo para thr_Oxigenio.

unsigned int linhas_geradas = 0;  // Quantidade de linhas geradas ate o momento.
unsigned int linhas_total = 0;    // Quantidade total de linhas a serem geradas.

// Gera N_HIDROGENIO atomos de hidrogenio.
void *thr_Hidrogenio(void *arg) {
    unsigned int i = 0;
    for (;;) {
        if (i == 0) {
            pthread_mutex_lock(&mutex_h);          // Aguarda o semaforo para thr_Hidrogenio ser liberado em thr_Oxigenio.
            if (linhas_geradas == linhas_total) {  // Ao gerar o total de linhas, libera o semaforo para thr_Enxofre e encerra.
                //printf("\nThread thr_Hidrogenio encerrada.");
                pthread_mutex_unlock(&mutex_s);
                pthread_exit(NULL);
            }
        }
        if (i >= N_HIDROGENIO) {
            i = 0;
            pthread_mutex_unlock(&mutex_s);  // Libera o semaforo para thr_Enxofre.
        } else {
            printf("H");
            ++i;
        }
    }
}

// Gera N_ENXOFRE atomos de enxofre.
void *thr_Enxofre(void *arg) {
    unsigned int i = 0;
    for (;;) {
        if (i == 0) {
            pthread_mutex_lock(&mutex_s);          // Aguarda o semaforo para thr_Enxofre ser liberado em thr_Oxigenio.
            if (linhas_geradas == linhas_total) {  // Ao gerar o total de linhas, libera o semaforo para thr_Oxigenio e encerra.
                //printf("\nThread thr_Enxofre encerrada.");
                pthread_mutex_unlock(&mutex_o);
                pthread_exit(NULL);
            }
        }
        if (i >= N_ENXOFRE) {
            i = 0;
            pthread_mutex_unlock(&mutex_o);  // Libera o semaforo para thr_Oxigenio.
        } else {
            printf("S");
            ++i;
        }
    }
}

// Gera N_OXIGENIO atomos de oxigenio.
void *thr_Oxigenio(void *arg) {
    unsigned int i = 0;
    for (;;) {
        if (i == 0) {
            pthread_mutex_lock(&mutex_o);          // Aguarda o semaforo para thr_Enxofre ser liberado em thr_Hidrogenio.
            if (linhas_geradas == linhas_total) {  // Ao gerar o total de linhas, libera o semaforo para thr_Oxigenio e encerra.
                //printf("\nThread thr_Oxigenio encerrada.");
                pthread_mutex_unlock(&mutex_h);
                pthread_exit(NULL);
            }
        }
        if (i >= N_OXIGENIO) {
            i = 0;
            ++linhas_geradas;
            putchar('\n');
            pthread_mutex_unlock(&mutex_h);  // Libera o semaforo para thr_Hidrogenio.
        } else {
            printf("O");
            ++i;
        }
    }
}

//HHSOOOO

int main() {
    pthread_t thread_h, thread_s, thread_o;

    // Solicita o numero de linhas a serem geradas.
    printf("Informe o numero de linhas: ");
    scanf("%d", &linhas_total);

    // Bloqueia o semaforo para as threads enxofre e oxigenio
    // para garantir que a thread hidrogenio execute primeiro.
    pthread_mutex_lock(&mutex_s);
    pthread_mutex_lock(&mutex_o);

    // Cria as threads.
    if (pthread_create(&thread_h, NULL, thr_Hidrogenio, NULL) ||
        pthread_create(&thread_s, NULL, thr_Enxofre, NULL) ||
        pthread_create(&thread_o, NULL, thr_Oxigenio, NULL)) {
        fprintf(stderr, "\n\nFalha ao criar threads.\n\n");
        exit(-1);
    }

    pthread_exit(NULL);

    system("pause");
}
