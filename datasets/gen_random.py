#!/usr/bin/python3

import numpy as np
from sklearn import datasets


n_samples = 1500
noisy_circles = np.random.rand(n_samples, 2)


with open("random.in", "w") as f:
    print(n_samples, file=f)
    for sample in noisy_circles:
        print(*sample, file=f)
