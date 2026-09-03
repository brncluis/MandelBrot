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
    char **linhas_texto;
    int *proxima_linha_formato;
    pthread_mutex_t *mutex_formato;
} ArgsFormatacao;

void formatar_linha(ArgsFormatacao *dados, int linha) {

    int largura = dados->largura;

    char *buffer = malloc((size_t) largura * 4 + 2);

    if (buffer == NULL) {
        reportar_erro("Erro ao alocar memoria para formatar linha (pthreads2)");
        return;
    }

    int pos = 0;

    for (int coluna = 0; coluna < largura; coluna++) {

        int valor = dados->imagem[linha * largura + coluna];
        pos += sprintf(buffer + pos, "%d", valor);

        if (coluna < largura - 1) {
            buffer[pos++] = ' ';
        }

    }

    buffer[pos++] = '\n';
    buffer[pos] = '\0';

    dados->linhas_texto[linha] = buffer;

}

void *thread_formatadora(void *arg) {

    ArgsFormatacao *dados = (ArgsFormatacao *)arg;

    while (1) {

        pthread_mutex_lock(dados->mutex_formato);

        int linha = *dados->proxima_linha_formato;

        if (linha >= dados->altura) {
            pthread_mutex_unlock(dados->mutex_formato);
            break;
        }

        *dados->proxima_linha_formato = linha + 1;

        pthread_mutex_unlock(dados->mutex_formato);

        formatar_linha(dados, linha);

    }

    return NULL;

}

int rodar_pthreads2(int largura, int altura, int max_iteracao, int qtd_threads) {

    unsigned char *imagem = malloc((size_t) largura * altura * sizeof(unsigned char));

    if (imagem == NULL) {
        reportar_erro("Criacao da imagem falhou (pthreads2)");
        return 1;
    }

    char **linhas_texto = calloc((size_t) altura, sizeof(char *));

    if (linhas_texto == NULL) {
        reportar_erro("Erro ao alocar memoria para linhas de texto (pthreads2)");
        free(imagem);
        return 1;
    }

    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (int linha = 0; linha < altura; linha++) {
        calcular_cor(imagem, linha, largura, altura, max_iteracao);
    }

    int proxima_linha_formato = 0;
    pthread_mutex_t mutex_formato;
    pthread_mutex_init(&mutex_formato, NULL);

    ArgsFormatacao dados;
    dados.imagem = imagem;
    dados.largura = largura;
    dados.altura = altura;
    dados.linhas_texto = linhas_texto;
    dados.proxima_linha_formato = &proxima_linha_formato;
    dados.mutex_formato = &mutex_formato;

    pthread_t *threads = malloc((size_t) qtd_threads * sizeof(pthread_t));

    if (threads == NULL) {
        reportar_erro("Erro ao alocar memoria para as threads (pthreads2)");
        pthread_mutex_destroy(&mutex_formato);
        free(linhas_texto);
        free(imagem);
        return 1;
    }

    for (int i = 0; i < qtd_threads; i++) {

        int retorno = pthread_create(&threads[i], NULL, thread_formatadora, &dados);

        if (retorno != 0) {
            reportar_erro("Erro ao criar thread formatadora (pthreads2)");
            free(threads);
            pthread_mutex_destroy(&mutex_formato);
            free(linhas_texto);
            free(imagem);
            return 1;
        }

    }

    for (int i = 0; i < qtd_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);

    pthread_mutex_destroy(&mutex_formato);

    clock_gettime(CLOCK_MONOTONIC, &fim);

    FILE *arquivo_pgm = fopen("mandelbrot_lhass_pthreads2.pgm", "w");

    if (arquivo_pgm == NULL) {
        reportar_erro("Erro ao gerar pgm pthreads2");
        for (int linha = 0; linha < altura; linha++) {
            free(linhas_texto[linha]);
        }
        free(linhas_texto);
        free(imagem);
        return 1;
    }

    for (int linha = 0; linha < altura; linha++) {
        fputs(linhas_texto[linha], arquivo_pgm);
        free(linhas_texto[linha]);
    }

    fclose(arquivo_pgm);
    free(linhas_texto);

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