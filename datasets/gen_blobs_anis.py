import numpy as np
from sklearn import datasets


n_samples = 1500
random_state = 170
noisy_circles, labels = datasets.make_blobs(n_samples=n_samples, random_state=random_state)
transformation = [[0.6, -0.6], [-0.4, 0.8]]
noisy_circles = np.dot(noisy_circles, transformation)

with open("blobs_anis.in", "w") as f:
    print(n_samples, file=f)
    for sample in noisy_circles:
        print(*sample, file=f)

with open("blobs_anis.lbl", "w") as f:
    for label in labels:
        print(label, file=f)