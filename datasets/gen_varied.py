#!/usr/bin/python3

import numpy as np
from sklearn import datasets


n_samples = 1500
random_state = 170
noisy_circles, labels = datasets.make_blobs(n_samples=n_samples, cluster_std=[1.0, 2.5, 0.5], random_state=random_state)


with open("varied.in", "w") as f:
    print(n_samples, file=f)
    for sample in noisy_circles:
        print(*sample, file=f)

with open("varied.lbl", "w") as f:
    for label in labels:
        print(label, file=f)
