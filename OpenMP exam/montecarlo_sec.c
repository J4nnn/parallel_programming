//
// Created by j4nn on 10/27/26.
//

/* montecarlo.c -- versión secuencial de referencia.
   Uso: ./montecarlo <numero de muestras> */

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  long N = (argc > 1) ? atol(argv[1]) : 100000000L;

  unsigned int estado = 2210048U;
  long dentro = 0;
  double t_inicial = omp_get_wtime();

  for (long i = 0; i < N; i++) {
    double x = rand_r(&estado) / (double)RAND_MAX;
    double y = rand_r(&estado) / (double)RAND_MAX;

    if (x * x + y * y <= 1.0) {
      dentro++;
    }
  }
  double t_final = omp_get_wtime();
  double tiempo = t_final - t_inicial;

  printf("Estimacion de pi: %.6f\n", 4.0 * dentro / (double)N);
  printf("Tiempo: %.6f\n", tiempo);

  return 0;
}
