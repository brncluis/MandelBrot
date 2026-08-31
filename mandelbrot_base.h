#ifndef MANDELBROT_BASE_H
#define MANDELBROT_BASE_H

int parse_inteiro_positivo(char *texto, int *saida);

void reportar_erro(char *mensagem);

int mandelbrot_ponto(double c_real, double c_imag, int max_iteracao);

void calcular_cor(unsigned char *imagem, int linha, int largura, int altura, int max_iteracao);

int gera_pgm(unsigned char *imagem, int largura, int altura, char *nome);

int rodar_serial(int largura, int altura, int max_iteracao, int qtd_threads);
int rodar_openmp(int largura, int altura, int max_iteracao, int qtd_threads);
int rodar_pthreads1(int largura, int altura, int max_iteracao, int qtd_threads);
int rodar_pthreads2(int largura, int altura, int max_iteracao, int qtd_threads);

#endif