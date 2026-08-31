#define _POSIX_C_SOURCE 199309L

#include "mandelbrot_base.h"

#include <stdio.h>      
#include <stdlib.h>     
#include <time.h> 
#include <pthread.h>

typedef struct {
    unsigned char *imagem;
    int largura;
    int altura;
    int max_iteracao;
    int *proxima_linha;
    pthread_mutex_t *mutex;
} ArgsThreadsDinamico;

void *thread_dinamica(void *arg) {

    ArgsThreadsDinamico *dados = (ArgsThreadsDinamico*)arg;

    while (1) {

        pthread_mutex_lock(dados->mutex);

        int linha_atual = *dados->proxima_linha;

        if (linha_atual >= dados->altura) {
            pthread_mutex_unlock(dados->mutex);
            break;
        }

        *dados->proxima_linha = linha_atual + 1;

        pthread_mutex_unlock(dados->mutex);

        calcular_cor(dados->imagem, linha_atual, dados->largura, dados->altura, dados->max_iteracao);
    }

    return NULL;

}

int rodar_pthreads2(int largura, int altura, int max_iteracao, int qtd_threads) {

    unsigned char *imagem = malloc((size_t) largura * altura * sizeof(unsigned char));

    if (imagem == NULL) {
        reportar_erro("Criacao da imagem falhou (pthreads2)");
        return 1;
    }

    int proxima_linha = 0;
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    ArgsThreadsDinamico *args = malloc((size_t) qtd_threads * sizeof(ArgsThreadsDinamico));
    pthread_t *threads = malloc((size_t) qtd_threads * sizeof(pthread_t));

    if (args == NULL || threads == NULL) {
        reportar_erro("Erro ao alocar memoria para as threads (pthreads2)");
        free(args);
        free(threads);
        pthread_mutex_destroy(&mutex);
        free(imagem);
        return 1;
    }


    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (int i = 0; i < qtd_threads; i++) {

    args[i].imagem = imagem;
    args[i].largura = largura;
    args[i].altura = altura;
    args[i].max_iteracao = max_iteracao;
    args[i].proxima_linha = &proxima_linha;
    args[i].mutex = &mutex;

    int retorno = pthread_create(&threads[i], NULL, thread_dinamica, &args[i]);

        if (retorno != 0) {
            reportar_erro("Erro ao criar thread (pthreads2)");
            free(args);
            free(threads);
            pthread_mutex_destroy(&mutex);
            free(imagem);
            return 1;
        }
    }

    for (int i = 0; i < qtd_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    free(args);
    free(threads);

    pthread_mutex_destroy(&mutex);

    clock_gettime(CLOCK_MONOTONIC, &fim);

    int retorno_gerar = gera_pgm(imagem, largura, altura, "mandelbrot_lhass_pthreads2.pgm");

    if (retorno_gerar != 0) {
        reportar_erro("Erro ao gerar pgm (pthreads2)");
        free(imagem);
        return 1;
    }

    double tempo_gasto = (fim.tv_sec - inicio.tv_sec) + ((fim.tv_nsec - inicio.tv_nsec) / 1e9);

    FILE *arquivo_tempo = fopen("times.txt", "a");

    if (arquivo_tempo == NULL) {
        reportar_erro("Erro ao abrir times.txt (pthreads2)");
        free(imagem);
        return 1;
    }

    fprintf(arquivo_tempo, "Pthreads2: %fs\n", tempo_gasto);
    fclose(arquivo_tempo);

    free(imagem);

    return 0;

}
