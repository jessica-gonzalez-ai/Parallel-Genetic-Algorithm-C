# Parallel Genetic Algorithm (GA) with Pthreads 🧬

A robust implementation of a **Genetic Algorithm** developed in **C**, featuring a multi-threaded approach using the **POSIX Threads (pthreads)** library to optimize fitness evaluation and selection processes.

##  Key Features
* **Multi-threaded Execution:** Configured to run with **8 concurrent threads** to maximize CPU utilization.
* **Scalable Architecture:** Designed to handle complex fitness functions with reduced execution time.
* **Performance Comparison:** Includes both serial (`ag.c`) and parallel (`agparalelo.c`) versions for benchmarking.

##  Technical Stack
* **Language:** C
* **Concurrency:** POSIX Threads (pthreads)
* **Optimization:** Genetic operators (Selection, Crossover, Mutation).

## 📁 File Descriptions
* `agparalelo.c`: The core parallel implementation utilizing 8 threads.
* `ag.c`: The standard serial version of the Genetic Algorithm.
* `mt.h` & `alea.h`: Libraries for statistical procedures and random population generation.

##  Results
The parallel version achieves a significant reduction in computational time compared to the serial approach, demonstrating the efficiency of shared-memory parallelism in evolutionary computation.
