#include "mandelbrot_base.h"

#include <stdio.h>

int main(int argc, char *argv[]) {

    FILE *arquivo_times = fopen("times.txt", "w");
 
        if (arquivo_times != NULL) {
            fclose(arquivo_times);
        }
 
    FILE *arquivo_erros = fopen("erros.txt", "w");
 
        if (arquivo_erros != NULL) {
            fclose(arquivo_erros);
        }

    if (argc != 5) {
        reportar_erro("Erro insira mandelbrot largura altura iteracoes threads");
        return 1;
    }

    int largura, altura, max_iteracao, qtd_threads;

    if (parse_inteiro_positivo(argv[1], &largura) != 0 ||
        parse_inteiro_positivo(argv[2], &altura) != 0 ||
        parse_inteiro_positivo(argv[3], &max_iteracao) != 0 ||
        parse_inteiro_positivo(argv[4], &qtd_threads) != 0) {

        reportar_erro("Passe apenas numeros inteiros positivos validos");
        return 1;

    }

    if (rodar_serial(largura, altura, max_iteracao, qtd_threads) != 0) {
        return 1;
    }

    if (rodar_openmp(largura, altura, max_iteracao, qtd_threads) != 0) {
        return 1;
    }

    if (rodar_pthreads1(largura, altura, max_iteracao, qtd_threads) != 0) {
        return 1;
    }

    if (rodar_pthreads2(largura, altura, max_iteracao, qtd_threads) != 0) {
        return 1;
    }

    return 0;

}
