import matplotlib
matplotlib.use('TkAgg')

import matplotlib.pyplot as plt
import numpy as np
import os
import subprocess


executable_path = "./test-concurrent-table"
thread_number = 50
ops_number = "1"
test_numbers = 10

try:
    thread_output = []
    runtime_output = []
    for _ in range(test_numbers):
        result = subprocess.run([executable_path, str(thread_number), ops_number], capture_output=True, text=True, check=True)
        thread_output.append(thread_number)
        runtime_output.append(result.stdout.split(',')[1])
        thread_number *= 2
    
    print("Executable output:")
    print(result.stdout)
    
except subprocess.CalledProcessError as e:
    print(f"Error running executable: {e}")
    print(f"Stderr: {e.stderr}")
except FileNotFoundError:
    print(f"Error: Executable not found at {executable_path}")


# Define your two parameters (variables) as lists or NumPy arrays
# Parameter 1 (x-axis data)
x = np.array(thread_output)
y = np.array(runtime_output)

plt.scatter(x, y)

plt.plot(x, y, linestyle='-', color='blue', label='Linear Line')


plt.xlabel("Threads")
plt.ylabel("Time (seconds)")
plt.title("Relationship between number of threads and ")



plt.show()