#define _POSIX_C_SOURCE 199309L

#include "mandelbrot_base.h"

#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>   

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

    for (int linha = 0; linha < altura; linha++) {
        calcular_cor(imagem, linha, largura, altura, max_iteracao);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    int retorno_gerar = gera_pgm(imagem, largura, altura, "mandelbrot_lhass_serial.pgm");

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

    fprintf(arquivo_tempo, "serial tempo: %f\n", tempo_gasto);
    fclose(arquivo_tempo);

    free(imagem);

    return 0;

    
}