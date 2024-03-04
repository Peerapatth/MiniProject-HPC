import matplotlib.pyplot as plt
import numpy as np


# Function to calculate Amdahl's speedup
def amdahl_speedup(N, S):
    return 1 / (S + (1 - S) / N)


def analytical_speedup(N, S, alpha):
    return 1 / (S + (1 - S) / N + alpha * (N - 1) / N)


# Function to plot the graph
def plot_graph(N, S, alpha, runtimes):
    # Calculate speedup
    ideal_speedups = [n for n in N]
    amdahl_speedups = [amdahl_speedup(n, S) for n in N]
    analytical_speedups = [analytical_speedup(n, S, alpha) for n in N]
    expermental_speedups = [runtimes[0] / runtime for runtime in runtimes]

    # Calculate efficiency
    ideal_efficiency = [1 for n in N]
    amdahl_efficiency = [amdahl_speedup(n, S) / n for n in N]
    analytical_efficiency = [analytical_speedup(n, S, alpha) / n for n in N]
    expermental_efficiency = [
        runtimes[0] / (n * runtime) for n, runtime in zip(N, runtimes)
    ]

    # Plot the graph
    plt.figure(figsize=(10, 6))
    # Plot the graph
    plt.plot(N, ideal_speedups, label="Ideal speedup", marker="o")
    plt.plot(N, amdahl_speedups, label="Amdahl's speedup", marker="o")
    plt.plot(N, analytical_speedups, label="Analytical speedup", marker="o")
    plt.plot(N, expermental_speedups, label="Experimental speedup", marker="o")
    plt.plot(N, ideal_efficiency, label="Ideal efficiency", linestyle="--")
    plt.plot(N, amdahl_efficiency, label="Amdahl's efficiency", linestyle="--")
    plt.plot(N, analytical_efficiency, label="Analytical efficiency", linestyle="--")
    plt.plot(N, expermental_efficiency, label="Experimental efficiency", linestyle="--")

    plt.xlabel("Number of processors")
    plt.ylabel("Speedup/Efficiency")
    plt.title("Amdah vs Analytical vs Experimental Speedup and Efficiency")
    plt.legend()
    plt.grid(True)
    plt.show()


# Define parameters
N = [1, 2, 4, 8]  # Number of processors
S = 0.1  # Fraction of serial code
alpha = 0.1  # Communication overhead

# Runtime with 1, 2, 4, 8 processors (example values)
runtimes = [0.152563, 0.089987, 0.055290, 0.039725]

# Plot the graph
plot_graph(N, S, alpha, runtimes)
