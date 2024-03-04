# Parallel Quick Sort

Use C language with MPI and OpenMP Library to build a parallel quick sort.

## Performance Analysis

Analyze performance of your parallel application, using Amdahl’s model and analytical model.

- **Amdahl's Law:** This model considers a fixed fraction (S) of the code as inherently serial (cannot be parallelized). The remaining fraction (1 - S) is assumed to be perfectly parallelizable. It provides an upper bound on the achievable speedup with the following formula:
  `Speedup = 1 / (S + (1 - S) / N)`
- **Analytical Model:** This model introduces an additional parameter, alpha (α), representing the communication overhead between processors. This overhead increases with the number of processors, impacting the overall speedup. The analytical model uses the following formula:
  `Speedup = 1 / (S + (1 - S) / N + α * (N - 1) / N)`

## Graphs

![Screenshot](Figure.png)
Draw the Speed-up and Efficiency graph: must show an ideal line, Amdahl’s line, analytical line, and experimental runtime line with 1, 2, 4, 8 number of processors (cores/ranks).

## Computer System Architecture

Detail and draw diagram for computer system architecture of the experiment.

- Processor
  - Apple silicon M1
- Memory
  - 8GB
- Storage
  - 256GB
- Operating System
  - MacOS Sonoma 14.3.1 (23D60)

## Conclusion:

- Amdahl's Law offers a simple and quick estimate of the theoretical upper bound on speedup due to parallelization.
- While useful for initial assessments, Amdahl's Law might be overly optimistic due to its idealized assumptions.
- The analytical model provides a more realistic picture by incorporating communication overhead, resulting in a closer prediction of actual performance compared to Amdahl's Law.
- Experimental measurements remain the most accurate way to assess the actual performance improvement of a parallelized program.
- Choosing the most appropriate model depends on the desired level of accuracy and the resources available. Amdahl's Law provides a quick and easy estimate, while the analytical model offers a more realistic prediction with additional complexity. However, the most accurate picture comes from actual experimentation.
