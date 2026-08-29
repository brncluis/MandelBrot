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
    int linha_inicio;
    int linha_fim;
} ArgsThreads;

void *funcao_thread(void *arg) {

    ArgsThreads *dados_threads = (ArgsThreads *)arg;

    for (int linha = dados_threads->linha_inicio; linha < dados_threads->linha_fim; linha++) {

        calcular_cor(dados_threads->imagem, linha, dados_threads->largura, dados_threads->altura, dados_threads->max_iteracao);

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


    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    ArgsThreads array_threads[qtd_threads];

    pthread_t threads[qtd_threads];

    int linhas_base = altura / qtd_threads;
    int resto = altura % qtd_threads;

    int linha_atual = 0;

    for (int i = 0; i < qtd_threads; i++) {

        int linhas_dessa_thread = linhas_base;

        if (i < resto) {
            linhas_dessa_thread += 1;
        }

        array_threads[i].imagem = imagem;
        array_threads[i].largura = largura;
        array_threads[i].altura = altura;
        array_threads[i].max_iteracao = max_iteracao;
        array_threads[i].linha_inicio = linha_atual;
        array_threads[i].linha_fim = linha_atual + linhas_dessa_thread;

        linha_atual += linhas_dessa_thread;

        int retorno = pthread_create(&threads[i], NULL, funcao_thread, &array_threads[i]);

        if (retorno != 0) {
            fprintf(stderr, "Erro p criar a thread\n");
            free(imagem);
            return 1;
        }

    }

    for (int i = 0; i < qtd_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    int retorno_gerar = gera_pgm(imagem, largura, altura, "mandelbrot_lhass_pthreads1.pgm");

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

    fprintf(arquivo_tempo, "pthreads1 tempo: %f\n", tempo_gasto);
    fclose(arquivo_tempo);

    free(imagem);

    return 0;

}
