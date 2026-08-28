#define _POSIX_C_SOURCE 199309L

#include "mandelbrot_base.h"

#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>   

int mandelbrot_ponto(double c_real, double c_imag, int max_iteracao) {
    double z_real = 0.0;
    double z_imag = 0.0;

    for (int i = 0; i < max_iteracao; i++) {

        double distancia_ao_quadrado = z_real * z_real + z_imag * z_imag;
        if (distancia_ao_quadrado > 4.0) {
            return i;  
        }

        double z_real_antigo = z_real;

        z_real = (z_real * z_real - z_imag * z_imag) + c_real;
        z_imag = (2.0 * z_real_antigo * z_imag) + c_imag;
    }

    return max_iteracao;
}

void calcular_cor(unsigned char *imagem, int linha, int largura, int altura, int max_iteracao) {

    double c_imag = -1.5 + ((double) linha / (altura - 1)) * 3.0;

    for (int coluna = 0; coluna < largura; coluna++) {

        double c_real = -2.0 + ((double) coluna / (largura - 1)) * 3.0;

        int iteracoes = mandelbrot_ponto(c_real, c_imag, max_iteracao);
        unsigned char intensidade = (unsigned char) (255.0 * iteracoes / max_iteracao);

        int posicao_no_array = linha * largura + coluna;
        imagem[posicao_no_array] = intensidade;

    }

}

int gera_pgm(unsigned char *imagem, int largura, int altura, char *nome) {

    FILE *arquivo = fopen(nome, "w");

    if (arquivo == NULL) {
        return 1;
    }

    for (int linha = 0; linha < altura; linha++) {
        for (int coluna = 0; coluna < largura; coluna++) {
            int posicao_lista = linha * largura + coluna;
            fprintf(arquivo, "%d ", imagem[posicao_lista]);
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);

    return 0;

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