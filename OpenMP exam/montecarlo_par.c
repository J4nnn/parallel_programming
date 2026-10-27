/* montecarlo.c -- versión paralela con OpenMP.
   Uso: ./montecarlo [numero de muestras] [numero de hilos] */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    long N = (argc > 1) ? atol(argv[1]) : 100000000L;
    if (argc > 2)
        omp_set_num_threads(atoi(argv[2]));

    unsigned int estado = 2210048u;   /* semilla asignada */
    long dentro = 0;

    double t0 = omp_get_wtime();

    #pragma omp parallel for reduction(+:dentro)
    for (long i = 0; i < N; i++) {
        double x = rand_r(&estado) / (double)RAND_MAX;
        double y = rand_r(&estado) / (double)RAND_MAX;
        if (x * x + y * y <= 1.0)
            dentro++;
    }

    double t1 = omp_get_wtime();

    printf("Estimacion de pi: %.6f\n", 4.0 * dentro / (double)N);
    printf("hilos=%d dentro=%ld tiempo=%.4f\n", omp_get_max_threads(), dentro, t1 - t0);
    return 0;
}
