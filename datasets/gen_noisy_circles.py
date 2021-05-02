#!/usr/bin/python3

# For more info, see
# https://scikit-learn.org/stable/auto_examples/cluster/plot_cluster_comparison.html

import numpy as np
from sklearn import datasets


n_samples = 1500
noisy_circles, labels = datasets.make_circles(n_samples=n_samples, factor=0.5, noise=0.05)

with open("noisy_circles.in", "w") as f:
    print(n_samples, file=f)
    for sample in noisy_circles:
        print(*sample, file=f)

with open("noisy_circles.lbl", "w") as f:
    for label in labels:
        print(label, file=f)
