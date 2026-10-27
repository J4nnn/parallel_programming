/* montecarlo.c -- estimacion de pi por Monte Carlo (acierto y fallo) con
   OpenMP. Uso: ./montecarlo [numero de muestras] [numero de hilos] Si no se da
   el numero de hilos, se usa OMP_NUM_THREADS. */
#include <math.h>
#include <omp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define SEMILLA 2210048ULL          /* semilla asignada */
#define GAMMA 0x9E3779B97F4A7C15ULL /* incremento de SplitMix64 */

/* Generador basado en contador (SplitMix64).
   El numero en la posicion k del flujo depende solo de (SEMILLA, k):
   no hay estado compartido entre hilos y no importa quien lo calcule. */
static inline uint64_t aleatorio(uint64_t k) {
  uint64_t z = SEMILLA + k * GAMMA;
  z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
  z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
  return z ^ (z >> 31);
}

/* Convierte 64 bits aleatorios en un double uniforme en [0, 1) (53 bits). */
static inline double uniforme(uint64_t r) {
  return (double)(r >> 11) * 0x1.0p-53;
}

int main(int argc, char *argv[]) {
  long N = (argc > 1) ? atol(argv[1]) : 100000000L;
  if (argc > 2)
    omp_set_num_threads(atoi(argv[2]));

  long dentro = 0;
  int p = omp_get_max_threads(); /* hilos que usara la region paralela */

  double t0 = omp_get_wtime();

/* Cada muestra i usa las posiciones 2i y 2i+1 del flujo.
   Con schedule(static) el hilo t recibe las muestras [t*N/p, (t+1)*N/p),
   o sea un bloque de 2N/p posiciones que no se solapa con el de otro hilo. */
#pragma omp parallel for default(none) shared(N) reduction(+ : dentro)         \
    schedule(static)
  for (long i = 0; i < N; i++) {
    double x = uniforme(aleatorio(2 * (uint64_t)i));
    double y = uniforme(aleatorio(2 * (uint64_t)i + 1));
    if (x * x + y * y <= 1.0)
      dentro++;
  }

  double t1 = omp_get_wtime();

  double pi = 4.0 * dentro / (double)N;
  printf("Estimacion de pi: %.6f\n", pi);
  printf("hilos=%d N=%ld dentro=%ld error=%.2e tiempo=%.4f\n", p, N, dentro,
         fabs(pi - M_PI), t1 - t0);
  return 0;
}
