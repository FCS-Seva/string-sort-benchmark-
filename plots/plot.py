import csv
import os
from collections import defaultdict

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

CSV_PATH = os.path.join(os.path.dirname(__file__), "..", "results", "measurements.csv")
OUT_DIR = os.path.dirname(__file__)

ALGO_ORDER = [
    "std_quicksort",
    "std_mergesort",
    "string_quicksort",
    "string_mergesort_lcp",
    "msd_radix",
    "msd_radix_quick",
]

ALGO_LABEL = {
    "std_quicksort":        "Std QuickSort",
    "std_mergesort":        "Std MergeSort",
    "string_quicksort":     "String QuickSort (3-way)",
    "string_mergesort_lcp": "String MergeSort + LCP",
    "msd_radix":            "MSD Radix",
    "msd_radix_quick":      "MSD Radix + QuickSort",
}

TYPE_TITLE = {
    "random":  "Случайные строки",
    "reverse": "Обратно отсортированный",
    "nearly":  "Почти отсортированный",
}


def load_data(path):
    data = defaultdict(lambda: defaultdict(list))
    with open(path, newline="") as f:
        reader = csv.DictReader(f)
        for row in reader:
            data[row["type"]][row["algorithm"]].append((
                int(row["n"]),
                float(row["time_us"]),
                int(row["char_comparisons"]),
            ))
    for t in data:
        for a in data[t]:
            data[t][a].sort()
    return data


def plot_metric(data, type_name, metric_idx, ylabel, title_suffix, out_name, logy=False):
    plt.figure(figsize=(9, 5.5))
    subset = data[type_name]
    for algo in ALGO_ORDER:
        if algo not in subset:
            continue
        rows = subset[algo]
        xs = [r[0] for r in rows]
        ys = [r[metric_idx] for r in rows]
        plt.plot(xs, ys, marker="o", markersize=3, linewidth=1.4,
                 label=ALGO_LABEL[algo])
    plt.xlabel("Размер массива N")
    plt.ylabel(ylabel)
    plt.title(f"{TYPE_TITLE.get(type_name, type_name)} — {title_suffix}")
    if logy:
        plt.yscale("log")
    plt.grid(True, alpha=0.3)
    plt.legend(fontsize=9)
    plt.tight_layout()
    out_path = os.path.join(OUT_DIR, out_name)
    plt.savefig(out_path, dpi=150)
    plt.close()
    print(f"  saved {out_path}")


def main():
    if not os.path.exists(CSV_PATH):
        print(f"ERROR: {CSV_PATH} not found. Run the benchmark program first.")
        return

    data = load_data(CSV_PATH)
    print("Building plots...")
    for t in data.keys():
        plot_metric(data, t, 1, "Время, мкс", "время выполнения",
                    f"time_{t}.png")
        plot_metric(data, t, 2, "Кол-во посимвольных сравнений",
                    "посимвольные сравнения", f"cmps_{t}.png")

    plt.figure(figsize=(9, 5.5))
    for t in data.keys():
        algo = "string_quicksort"
        if algo not in data[t]:
            continue
        rows = data[t][algo]
        xs = [r[0] for r in rows]
        ys = [r[1] for r in rows]
        plt.plot(xs, ys, marker="o", markersize=3, label=f"{TYPE_TITLE[t]}")
    plt.xlabel("Размер массива N")
    plt.ylabel("Время, мкс")
    plt.title("String QuickSort: время в зависимости от типа массива")
    plt.grid(True, alpha=0.3)
    plt.legend()
    plt.tight_layout()
    plt.savefig(os.path.join(OUT_DIR, "string_quicksort_by_type.png"), dpi=150)
    plt.close()
    print(f"  saved {os.path.join(OUT_DIR, 'string_quicksort_by_type.png')}")
    print("Done.")


if __name__ == "__main__":
    main()
