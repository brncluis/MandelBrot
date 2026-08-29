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

int main(int argc, char *argv[]){

    if (argc != 5) {
        fprintf(stderr, "Erro insira mandelbrot largura altura iterações threads\n");
        return 1;
    }

    int largura = atoi(argv[1]);
    int altura = atoi(argv[2]);
    int max_iteracao = atoi(argv[3]);
    int qtd_threads = atoi(argv[4]);

    if (largura <= 0 || altura <= 0 || max_iteracao <= 0 || qtd_threads <= 0) {

        fprintf(stderr, "Passe apenas numeros positivos\n");
        return 1;

    }

    unsigned char *imagem = malloc((size_t) largura * altura * sizeof(unsigned char));

    if (imagem == NULL) {
        fprintf(stderr, "Criacao da imagem falhou\n");
        return 1;
    }

    int proxima_linha = 0;
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    ArgsThreadsDinamico args[qtd_threads];
    pthread_t threads[qtd_threads];


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
            fprintf(stderr, "Erro ao criar thread\n");
            free(imagem);
            return 1;
        }
    }

    for (int i = 0; i < qtd_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    clock_gettime(CLOCK_MONOTONIC, &fim);

    int retorno_gerar = gera_pgm(imagem, largura, altura, "mandelbrot_lhass_pthreads2.pgm");

    if (retorno_gerar != 0) {
        fprintf(stderr, "Erro ao gerar pgm\n");
        free(imagem);
        return 1;
    }

    double tempo_gasto = (fim.tv_sec - inicio.tv_sec) + ((fim.tv_nsec - inicio.tv_nsec) / 1e9);

    FILE *arquivo_tempo = fopen("times.txt", "a");

    if (arquivo_tempo == NULL) {
        fprintf(stderr, "Erro ao abrir times.txt\n");
        free(imagem);
        return 1;
    }

    fprintf(arquivo_tempo, "pthreads2 tempo: %f\n", tempo_gasto);
    fclose(arquivo_tempo);

    free(imagem);

    return 0;

}
