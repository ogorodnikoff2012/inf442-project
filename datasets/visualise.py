import numpy as np
import matplotlib.pyplot as plt
import utils

FILENAME = "random.out"

vars = utils.read_vars_from_file(FILENAME)

points = np.array(vars["points"])

if len(vars["outliers"]) > 0:
    plt.scatter(points[vars["outliers"], 0], points[vars["outliers"], 1], label="outliers")

for id, cluster in enumerate(vars["clusters"]):
    plt.scatter(points[cluster, 0], points[cluster, 1], label=f"Cluster #{id}")


plt.legend()
plt.show()
