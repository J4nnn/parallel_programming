#!/usr/bin/env bash
# bench.sh -- mide T(p) para p = 1, 2, 4, 8 con 5 repeticiones cada uno.
# Uso: ./bench.sh [N] [repeticiones]
set -euo pipefail

N=${1:-100000000}
REPS=${2:-5}

gcc -O2 -fopenmp montecarlo_sec.c -o sec -lm
gcc -O2 -fopenmp montecarlo_par.c -o par -lm

echo "p,rep,tiempo,pi" > resultados.csv
./sec "$N" 1 > /dev/null   # calentamiento, no se registra

for p in 1 2 4 8; do
    for r in $(seq 1 "$REPS"); do
        salida=$(./sec "$N" "$p")
        t=$(echo "$salida" | grep -o 'tiempo=[0-9.]*' | cut -d= -f2)
        pi=$(echo "$salida" | head -1 | awk '{print $4}')
        echo "$p,$r,$t,$pi" >> resultados.csv
        echo "p=$p rep=$r tiempo=$t pi=$pi"
    done
done

echo "Listo: resultados.csv"
