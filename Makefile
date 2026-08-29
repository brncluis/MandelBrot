CFLAGS = -Wall -Wextra

all: mandelbrot_serial mandelbrot_openmp mandelbrot_pthreads1 mandelbrot_pthreads2

mandelbrot_serial: mandelbrot_serial.c mandelbrot_base.c mandelbrot_base.h
	gcc $(CFLAGS) -o mandelbrot_serial mandelbrot_serial.c mandelbrot_base.c

mandelbrot_openmp: mandelbrot_openmp.c mandelbrot_base.c mandelbrot_base.h
	gcc $(CFLAGS) -fopenmp -o mandelbrot_openmp mandelbrot_openmp.c mandelbrot_base.c

mandelbrot_pthreads1: mandelbrot_pthreads1.c mandelbrot_base.c mandelbrot_base.h
	gcc $(CFLAGS) -o mandelbrot_pthreads1 mandelbrot_pthreads1.c mandelbrot_base.c -lpthread

mandelbrot_pthreads2: mandelbrot_pthreads2.c mandelbrot_base.c mandelbrot_base.h
	gcc $(CFLAGS) -o mandelbrot_pthreads2 mandelbrot_pthreads2.c mandelbrot_base.c -lpthread

clean:
	rm -f mandelbrot_serial mandelbrot_openmp mandelbrot_pthreads1 mandelbrot_pthreads2 *.pgm times.txt