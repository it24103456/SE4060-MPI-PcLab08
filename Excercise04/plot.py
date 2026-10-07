import re, statistics as st, matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
progs = {"../Excercise02/terminal_output.txt": "Exercise 2: Sum 1..10,000,000", "../Excercise03/terminal_output.txt": "Exercise 3: Monte Carlo Pi (10,000,000)"}
data = {}
for k in progs:
    d = {}
    for line in open(k, encoding="utf-8"):
        m = re.search(r"procs=(\d+).*time=([\d.]+)", line)
        if not m: continue
        d.setdefault(int(m[1]), []).append(float(m[2]))
    data[k] = {p: st.median(v) for p, v in sorted(d.items())}
for kind in ("time", "speedup"):
    fig, axs = plt.subplots(1, 2, figsize=(11, 4))
    for ax, (k, title) in zip(axs, progs.items()):
        d = data[k]; ps = list(d)
        if kind == "time":
            ax.plot(ps, [d[p] for p in ps], "o-"); ax.set_ylabel("Time (s)")
        else:
            ax.plot(ps, [d[1]/d[p] for p in ps], "o-", label="Measured")
            ax.plot(ps, ps, "--", color="gray", label="Ideal"); ax.set_ylabel("Speedup"); ax.legend()
        ax.set_xlabel("Number of processors"); ax.set_title(title, fontsize=10)
        ax.set_xticks(ps); ax.grid(alpha=.3)
    fig.tight_layout(); fig.savefig(f"{kind}_vs_procs.png", dpi=130)
