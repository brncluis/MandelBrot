# Mandelbrot

Gerador do conjunto de Mandelbrot em C, com implementações serial e paralelas (OpenMP e Pthreads).
Disciplina Infraestrutura de Software (CESAR School).
Desenvolvido por Luis Alves

## Arquivos utilizados

- `mandelbrot_base.h` / `mandelbrot_base.c` — funções compartilhadas pelas 4 implementações: validação de argumentos, cálculo do ponto no conjunto de Mandelbrot, cálculo de uma linha da imagem, e escrita do arquivo `.pgm`.
- `mandelbrot_serial.c` — versão sequencial (`rodar_serial`).
- `mandelbrot_openmp.c` — versão paralela com OpenMP (`rodar_openmp`).
- `mandelbrot_pthreads1.c` — versão paralela com Pthreads, divisão estática de linhas entre threads (`rodar_pthreads1`).
- `mandelbrot_pthreads2.c` — versão paralela com Pthreads, divisão dinâmica com fila protegida por mutex (`rodar_pthreads2`).
- `mandelbrot.c` — único `main` do programa: valida os argumentos e chama as 4 implementações acima em sequência.
- `Makefile` — regra de compilação do executável único `mandelbrot` e limpeza dos arquivos gerados.

## Sistema operacional

Desenvolvido e testado em **Ubuntu Linux**, arquitetura `x86_64`.

## Como compilar

```
make
```

Isso gera o executável único `mandelbrot`, compilando `mandelbrot.c`, `mandelbrot_base.c` e os 4 arquivos de implementação juntos.

Para recompilar do zero:

```
make clean
make
```

## Como executar

```
./mandelbrot [largura] [altura] [max_iteracoes] [num_threads]
```

Exemplo:

```
./mandelbrot 800 800 1000 4
```

Uma única execução roda as 4 implementações (serial, OpenMP, Pthreads estático, Pthreads dinâmico) em sequência, gera os 4 arquivos `.pgm` e registra o tempo de cada uma em `times.txt`. `num_threads` é usado pelas 3 versões paralelas; a versão serial recebe o mesmo valor mas o ignora, para manter uma única assinatura de argumentos.

## Como testar

Compare a saída do programa contra o conteúdo esperado de um caso de teste:

```
./mandelbrot 4 4 50 1
diff mandelbrot_lhass_serial.pgm esperado.txt
```

Sem diferença impressa, a saída bate com o esperado.

Também é possível verificar se as 4 implementações produzem a mesma imagem, comparando os `.pgm` gerados na mesma execução entre si:

```
diff mandelbrot_lhass_serial.pgm mandelbrot_lhass_openmp.pgm
diff mandelbrot_lhass_serial.pgm mandelbrot_lhass_pthreads1.pgm
diff mandelbrot_lhass_serial.pgm mandelbrot_lhass_pthreads2.pgm
```
