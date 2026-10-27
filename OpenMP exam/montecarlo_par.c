/* montecarlo.c -- version paralela con un generador privado por hilo.
   Uso: ./montecarlo [numero de muestras] [numero de hilos] */
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define SEMILLA 2210048u /* semilla asignada */

int main(int argc, char *argv[]) {
  long N = (argc > 1) ? atol(argv[1]) : 100000000L;
  if (argc > 2)
    omp_set_num_threads(atoi(argv[2]));

  long dentro = 0;

  double t0 = omp_get_wtime();

#pragma omp parallel default(none) shared(N) reduction(+ : dentro)
  {
    /* cada hilo tiene su propio estado, en su pila */
    unsigned int estado = SEMILLA + omp_get_thread_num();

#pragma omp for schedule(static)
    for (long i = 0; i < N; i++) {
      double x = rand_r(&estado) / (double)RAND_MAX;
      double y = rand_r(&estado) / (double)RAND_MAX;
      if (x * x + y * y <= 1.0)
        dentro++;
    }
  }

  double t1 = omp_get_wtime();

  printf("Estimacion de pi: %.6f\n", 4.0 * dentro / (double)N);
  printf("hilos=%d dentro=%ld tiempo=%.4f\n", omp_get_max_threads(), dentro,
         t1 - t0);
  return 0;
}
