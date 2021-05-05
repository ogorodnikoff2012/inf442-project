import numpy as np
from sklearn import datasets


n_samples = 1500
noisy_circles, labels = datasets.make_moons(n_samples=n_samples, noise=0.05)

with open("noisy_moons.in", "w") as f:
    print(n_samples, file=f)
    for sample in noisy_circles:
        print(*sample, file=f)

with open("noisy_moons.lbl", "w") as f:
    for label in labels:
        print(label, file=f)