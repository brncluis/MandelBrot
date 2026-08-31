CFLAGS = -Wall -Wextra

all: mandelbrot

mandelbrot: mandelbrot.c mandelbrot_base.c mandelbrot_serial.c mandelbrot_openmp.c mandelbrot_pthreads1.c mandelbrot_pthreads2.c mandelbrot_base.h
	gcc $(CFLAGS) -fopenmp -o mandelbrot mandelbrot.c mandelbrot_base.c mandelbrot_serial.c mandelbrot_openmp.c mandelbrot_pthreads1.c mandelbrot_pthreads2.c -lpthread

clean:
	rm -f mandelbrot *.pgm times.txt erros.txt
