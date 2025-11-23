import matplotlib
matplotlib.use('TkAgg')

import matplotlib.pyplot as plt
import numpy as np
import subprocess


executable_path = "./test-concurrent-table"
thread_number = 4
ops_number = "1"


def run_executable(exe_path):
    
    try:
        output = {}
        
        for i in range(thread_number):
            result = subprocess.run([exe_path, str(i+1), ops_number], capture_output=True, text=True, check=True)
            output.update({i+1: result.stdout.split(',')[1]})
        
        
    except subprocess.CalledProcessError as e:
        print(f"Error running executable: {e}")
        print(f"Stderr: {e.stderr}")
    except FileNotFoundError:
        print(f"Error: Executable not found at {exe_path}")
        
    return output

output_simple_counter = run_executable("./simple_counter")
simple_times = [float(time) for time in output_simple_counter.values()]
simple_ops = np.array(simple_times)


output_approx_counter = run_executable("./test-concurrent-table")
approx_times = [float(time) for time in output_approx_counter.values()]
approx_ops = np.array(approx_times)

threads = [thread_num + 1 for thread_num in range(thread_number)]




# Define your two parameters (variables) as lists or NumPy arrays
# Parameter 1 (x-axis data)
x = np.array(threads)

plt.plot(x, simple_ops, linestyle='-', color='blue', label='simple_counter')
plt.plot(x, approx_times, linestyle='-', color='grey', label='approx_counter')


plt.xlabel("Threads")
plt.ylabel("Time (seconds)")
plt.title("Relationship between number of threads and run times")



plt.show()