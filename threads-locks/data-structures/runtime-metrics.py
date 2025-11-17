import matplotlib
matplotlib.use('QtAgg')

import matplotlib.pyplot as plt
import numpy as np

# Define your two parameters (variables) as lists or NumPy arrays
# Parameter 1 (x-axis data)
x = np.array([5, 7, 8, 7, 2, 17])
y = np.array([99, 86, 87, 88, 111, 86])

plt.scatter(x, y)

plt.xlabel("X Parameter")
plt.ylabel("Y Parameter")
plt.title("Relationship between two parameters")

plt.show()