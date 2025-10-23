# Sorting Algorithms Benchmark

A lightweight C++ benchmarking tool for testing and comparing sorting algorithms such as `std::sort`, `insertion_sort`, and more.  
It supports configurable dataset sizes, data patterns, multiple trials, and timing summaries (best/average/worst).

---

## Features

- Benchmark multiple sorting algorithms with consistent test data.
- Generate datasets in various patterns:
  - `random` – fully random integers  
  - `reverse` – reverse-sorted array  
  - `nearly` – almost sorted (only a few elements out of place)  
  - `dups` – many duplicate values
- Adjustable input size, seed, and number of trials.
- Reports **best / average / worst** run times.
- Easily extendable: add new sorting algorithms in `sorting.cpp`.
- Cross-platform, written in **modern C++20**.

---

## Build Instructions

### Prerequisites
- **CMake ≥ 3.20**
- **C++20 compiler**
  - GCC ≥ 10 / Clang ≥ 12 / MSVC ≥ 2019

### Build

```bash
# Clone or copy project
cd sorting_algorithms

# Configure and build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
