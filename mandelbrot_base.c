#include "mandelbrot_base.h"

#include <stdio.h>      
#include <stdlib.h>     

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

    double c_imag = -1.5 + ((double) linha / altura) * 3.0;

    for (int coluna = 0; coluna < largura; coluna++) {

        double c_real = -2.0 + ((double) coluna / largura) * 3.0;

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

            fprintf(arquivo, "%d", imagem[posicao_lista]);

            if (coluna < largura - 1) {
                fprintf(arquivo, " ");
            }

        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);

    return 0;

}

