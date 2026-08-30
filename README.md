# Mandelbrot

Gerador do conjunto de Mandelbrot em C, com implementações serial e paralelas (OpenMP e Pthreads).
Disciplina Infraestrutura de Software (CESAR School).
Desenvolvido por Luis Alves

## Arquivos utilizados

- `mandelbrot_base.h` / `mandelbrot_base.c` — funções compartilhadas pelas 4 implementações: cálculo do ponto no conjunto de Mandelbrot, cálculo de uma linha da imagem, e escrita do arquivo `.pgm`.
- `mandelbrot_serial.c` — versão sequencial (`main`).
- `mandelbrot_openmp.c` — versão paralela com OpenMP (`main`).
- `mandelbrot_pthreads1.c` — versão paralela com Pthreads, divisão estática de linhas entre threads (`main`).
- `mandelbrot_pthreads2.c` — versão paralela com Pthreads, divisão dinâmica com fila protegida por mutex (`main`).
- `Makefile` — regras de compilação das 4 versões e limpeza dos arquivos gerados.

## Sistema operacional

Desenvolvido e testado em **Ubuntu Linux**, arquitetura `x86_64`.

## Como compilar

```
make
```

Isso gera os quatro executáveis (`mandelbrot_serial`, `mandelbrot_openmp`, `mandelbrot_pthreads1`, `mandelbrot_pthreads2`) a partir dos respectivos `.c` e de `mandelbrot_base.c`.

Para recompilar do zero:

```
make clean
make
```

## Como executar

Todas as versões recebem os mesmos quatro argumentos:

```
./mandelbrot_serial [largura] [altura] [max_iteracoes] [num_threads]
./mandelbrot_openmp [largura] [altura] [max_iteracoes] [num_threads]
./mandelbrot_pthreads1 [largura] [altura] [max_iteracoes] [num_threads]
./mandelbrot_pthreads2 [largura] [altura] [max_iteracoes] [num_threads]
```

Exemplo:

```
./mandelbrot_serial 800 800 1000 4
```

`num_threads` é ignorado na versão serial, mas continua sendo exigido para manter a mesma assinatura de linha de comando entre as 4 versões.

Cada execução gera um arquivo `mandelbrot_lhass_<versao>.pgm` com a imagem calculada, e adiciona uma linha em `times.txt` com o tempo de cálculo.

## Como testar

Compare a saída do programa contra o conteúdo esperado de um caso de teste:

```
./mandelbrot_serial 4 4 50 1
diff mandelbrot_lhass_serial.pgm esperado.txt
```

Sem diferença impressa, a saída bate com o esperado.

Também é possível verificar se as 4 implementações produzem a mesma imagem, rodando todas com os mesmos parâmetros e comparando os `.pgm` gerados entre si:

```
diff mandelbrot_lhass_serial.pgm mandelbrot_lhass_openmp.pgm
diff mandelbrot_lhass_serial.pgm mandelbrot_lhass_pthreads1.pgm
diff mandelbrot_lhass_serial.pgm mandelbrot_lhass_pthreads2.pgm
```