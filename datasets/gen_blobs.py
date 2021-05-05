import numpy as np
from sklearn import datasets


n_samples = 1500
noisy_circles, labels = datasets.make_blobs(n_samples=n_samples, random_state=8)

with open("blobs.in", "w") as f:
    print(n_samples, file=f)
    for sample in noisy_circles:
        print(*sample, file=f)

with open("blobs.lbl", "w") as f:
    for label in labels:
        print(label, file=f)