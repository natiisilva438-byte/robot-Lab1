"""Plot the five-bin histogram from the recorded Ubuntu GCC output."""

from __future__ import annotations

import csv
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


HERE = Path(__file__).resolve().parent
log_path = HERE / "evidence" / "direct_compile_result.txt"
if not log_path.exists():
    log_path = HERE / "direct_compile_result.txt"
lines = log_path.read_text(encoding="utf-8").splitlines()
sample_line = lines[1]
values = np.array([float(token) for token in sample_line.split()])
assert len(values) == 20

low = float(values.min())
high = float(values.max())
mean = float(values.mean())
assert abs(mean - 0.55804) < 1e-5
edges = np.linspace(low, high, 6)
counts, _ = np.histogram(values, bins=edges)
assert counts.sum() == 20

out = HERE / "figures"
out.mkdir(exist_ok=True)

with (out / "random_vector_samples.csv").open("w", newline="", encoding="utf-8") as stream:
    writer = csv.writer(stream)
    writer.writerow(["sample_index", "value"])
    writer.writerows((i, f"{value:.7g}") for i, value in enumerate(values, 1))

plt.rcParams.update({
    "font.family": "DejaVu Sans",
    "font.size": 10,
    "axes.spines.top": False,
    "axes.spines.right": False,
    "svg.fonttype": "none",
})
fig, ax = plt.subplots(figsize=(5.6, 3.25), dpi=300)
fig.patch.set_facecolor("white")
ax.set_facecolor("white")
widths = np.diff(edges)
bars = ax.bar(
    edges[:-1], counts, width=widths * 0.90, align="edge",
    color="#256d91", edgecolor="#1c4f68", linewidth=0.7,
)
for bar, count in zip(bars, counts):
    ax.text(bar.get_x() + bar.get_width() / 2, count + 0.15,
            str(int(count)), ha="center", va="bottom", fontsize=11, color="#153d52")

ax.axvline(mean, color="#b64b37", linewidth=1.5, linestyle="--",
           label=f"Mean = {mean:.3f}")
ax.scatter(values, np.full_like(values, -0.35), marker="|", s=75,
           linewidths=1.1, color="#163e53", clip_on=False, zorder=4)
ax.set_xlim(0, 1)
ax.set_ylim(0, max(counts) + 1.8)
ax.set_yticks(range(0, int(max(counts)) + 2, 2))
ax.set_xticks([0, 0.2, 0.4, 0.6, 0.8, 1.0])
ax.grid(axis="y", color="#e1e8ec", linewidth=0.65)
ax.set_axisbelow(True)
ax.set_xlabel("RandomVector value")
ax.set_ylabel("Sample count")
ax.set_title("RandomVector output distribution", loc="left", fontsize=13,
             fontweight="bold", pad=10)
ax.legend(frameon=False, loc="upper left", fontsize=10)
fig.text(0.12, 0.055,
         f"Ubuntu 20.04 / GCC 9.4  |  seed 314159  |  n = 20  |  5 equal-width bins",
         fontsize=9, color="#50616b")
fig.subplots_adjust(left=0.12, right=0.98, top=0.84, bottom=0.28)
fig.savefig(out / "random_vector_histogram.png", dpi=300, facecolor="white")
fig.savefig(out / "random_vector_histogram.svg", facecolor="white")
plt.close(fig)

print(f"samples={len(values)} range=({low:.7g}, {high:.7g}) mean={mean:.5f} bins={counts.tolist()}")
