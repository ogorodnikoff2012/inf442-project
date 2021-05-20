#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt
import utils

def draw(data, points):
    if len(data["outliers"]) > 0:
        plt.scatter(points[data["outliers"], 0], points[data["outliers"], 1], label="outliers")

    for i, cluster in enumerate(data["clusters"]):
        plt.scatter(points[cluster, 0], points[cluster, 1], label=f"Cluster #{i}")

    plt.legend()

targets = [
    ('blobs_anis', 5, 0.35),
    ('blobs', 5, 1.0),
    ('noisy_circles', 5, 0.15),
    ('noisy_moons', 5, 0.15),
    ('random', 5, 0.15),
    ('varied', 13, 0.925),
]

for target, n, eps in targets:
    out_data = utils.read_vars_from_file(target + '.out')
    pseudo_data = utils.read_vars_from_file(target + '.pseudo')

    points = np.array(out_data["points"])

    plt.figure(figsize=(16, 6))
    plt.suptitle(f"{target}, DBSCAN($N={n}$, $\\varepsilon={eps}$)")

    plt.subplot(121)
    plt.title("DBSCAN")
    draw(out_data, points)
    plt.subplot(122)
    plt.title("Pseudo-DBSCAN")
    draw(pseudo_data, points)
    plt.savefig(target + '.png')
