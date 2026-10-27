"""analizar.py -- lee resultados.csv, calcula medianas, S(p), E(p), f y Karp-Flatt,
y guarda la grafica speedup.png (si matplotlib esta instalado)."""

import csv
from statistics import median

tiempos = {}
with open("resultados.csv") as f:
    for fila in csv.DictReader(f):
        tiempos.setdefault(int(fila["p"]), []).append(float(fila["tiempo"]))

ps = sorted(tiempos)
T = {p: median(tiempos[p]) for p in ps}
S = {p: T[1] / T[p] for p in ps}
E = {p: S[p] / p for p in ps}

print("| p | Mediana T(p) [s] | S(p) | E(p) | Karp-Flatt e |")
print("|---|---|---|---|---|")
for p in ps:
    kf = "-" if p == 1 else f"{(1 / S[p] - 1 / p) / (1 - 1 / p):.4f}"
    print(f"| {p} | {T[p]:.4f} | {S[p]:.3f} | {E[p]:.3f} | {kf} |")

# Fraccion paralelizable f despejada de Amdahl con p = 4:
# S = 1 / ((1-f) + f/p)  =>  f = (1 - 1/S) / (1 - 1/p)
if 4 in S:
    f = (1 - 1 / S[4]) / (1 - 1 / 4)
    print(f"\nf estimado con p=4: {f:.4f}  (fraccion secuencial 1-f = {1 - f:.4f})")
    print(
        f"Amdahl con p=64: {1 / ((1 - f) + f / 64):.2f}   limite p->inf: "
        + (f"{1 / (1 - f):.1f}" if f < 1 else "inf")
    )

try:
    import matplotlib

    matplotlib.use("Agg")
    import matplotlib.pyplot as plt

    plt.plot(ps, [S[p] for p in ps], "o-", label="Speedup medido")
    plt.plot(ps, ps, "--", label="Ideal S(p) = p")
    if 4 in S:
        plt.plot(
            ps, [1 / ((1 - f) + f / p) for p in ps], ":", label=f"Amdahl (f = {f:.3f})"
        )
    plt.xlabel("Hilos p")
    plt.ylabel("Speedup S(p)")
    plt.xticks(ps)
    plt.grid(alpha=0.3)
    plt.legend()
    plt.title("Speedup Monte Carlo, N = 10^8")
    plt.savefig("speedup.png", dpi=150, bbox_inches="tight")
    print("\nGrafica guardada en speedup.png")
except ImportError:
    print(
        "\nmatplotlib no esta instalado: haz la grafica en Google Sheets con la tabla."
    )
