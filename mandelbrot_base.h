#ifndef MANDELBROT_BASE_H
#define MANDELBROT_BASE_H

int mandelbrot_ponto(double c_real, double c_imag, int max_iteracao);

void calcular_cor(unsigned char *imagem, int linha, int largura, int altura, int max_iteracao);

#endif