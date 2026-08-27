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

int main(int argc, char *argv[]){

    if (argc != 5) {
        printf("Erro insira mandelbrot largura altura iterações threads ");
    }

    int largura = atoi(argv[1]);
    int altura = atoi(argv[2]);
    int max_iteracao = atoi(argv[3]);
    int qtd_threads = atoi(argv[4]);

    if (largura <= 0 || altura <= 0 || max_iteracao <= 0 || qtd_threads <= 0) {

        printf("Passe apenas numeros positivos ");

    }

}