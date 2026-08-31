#define _POSIX_C_SOURCE 199309L

#include "mandelbrot_base.h"

#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>   

int rodar_serial(int largura, int altura, int max_iteracao, int qtd_threads) {

    (void) qtd_threads;

    unsigned char *imagem = malloc((size_t) largura * altura * sizeof(unsigned char));

    if (imagem == NULL) {
        reportar_erro("Criacao da imagem falhou (serial)");
        return 1;
    }


    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (int linha = 0; linha < altura; linha++) {
        calcular_cor(imagem, linha, largura, altura, max_iteracao);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    int retorno_gerar = gera_pgm(imagem, largura, altura, "mandelbrot_lhass_serial.pgm");

    if (retorno_gerar != 0) {
        reportar_erro("Erro ao gerar pgm (serial)");
        free(imagem);
        return 1;
    }

    double tempo_gasto = (fim.tv_sec - inicio.tv_sec) + ((fim.tv_nsec - inicio.tv_nsec) / 1e9);

    FILE *arquivo_tempo = fopen("times.txt", "a");

    if (arquivo_tempo == NULL) {
        reportar_erro("Erro ao abrir times.txt (serial)");
        free(imagem);
        return 1;
    }

    fprintf(arquivo_tempo, "Serial: %fs\n", tempo_gasto);
    fclose(arquivo_tempo);

    free(imagem);

    return 0;

    
}