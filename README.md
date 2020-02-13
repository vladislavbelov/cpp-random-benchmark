## cpp-random-benchmark

The goal of the benchmark is to have a rough comparison of implementations of random functions in C++ STL on the same platform.

### Results

**Platform:** `Windows 10, Intel Core i7-7800x 3.5GHz, DDR4 2400MHz`

1. **Compiler:** `g++.exe (i686-posix-dwarf-rev0, Built by MinGW-W64 project) 8.1.0`  
**Build command:** `g++ -O3 -std=c++17 -ffast-math cpp-random-benchmark.cpp`

2. **Compiler:** `Microsoft Visual C++ 2015 (Version 19.00.24215.1 for x86)`  
**Build command:** `cl /Ox /Ob1 /Oi /Ot cpp-random-benchmark.cpp`

##### Random Engines
| Random Engine Name | Compiler #1 (ns/call) | Compiler #2 (ns/call) |
|---|---:|---:|
| `std::default_random_engine` | 4.817 | 3.580 |
| `std::minstd_rand` | 3.985 | 15.287 |
| `std::minstd_rand0` | 4.827 | 15.249 |
| `std::mt19937` | 5.037 | 3.541 |
| `std::mt19937_64` | 6.491 | 7.360 |
| `std::ranlux24_base` | 4.588 | 7.002 |
| `std::ranlux48_base` | 4.548 | 8.342 |
| `std::ranlux24` | 53.904 | 70.064 |
| `std::ranlux48` | 232.295 | 278.260 |
| `std::knuth_b` | 10.083 | 16.309 |
| `RandEngine` | 8.402 | 18.607 |

##### Random Distributions

| Random Distribution Name | Compiler #1 (ns/call) | Compiler #2 (ns/call) |
|---|---:|---:|
| `std::uniform_int_distribution<std::size_t>(std::numeric_limits<std::size_t>::min(), std::numeric_limits<std::size_t>::max())` | 6.457 | 3.756 |
| `std::uniform_int_distribution<std::size_t>(0u, 1u)` | 7.751 | 12.402 |
| `std::uniform_int_distribution<int>(std::numeric_limits<int>::min(), std::numeric_limits<int>::max())` | 6.424 | 3.831 |
| `std::uniform_int_distribution<int>(0, 1)` | 7.671 | 11.525 |
| `std::uniform_int_distribution<int>(0, 7)` | 7.742 | 11.546 |
| `std::uniform_real_distribution<float>(std::numeric_limits<float>::min(), std::numeric_limits<float>::max())` | 5.047 | 53.962 |
| `std::uniform_real_distribution<float>(0.0f, 1.0f)` | 5.007 | 54.137 |
| `std::normal_distribution<float>(0.0f, 1.0f)` | 39.723 | 74.981 |
| `std::lognormal_distribution<float>(0.0f, 1.0f)` | 39.758 | 140.300 |
| `std::geometric_distribution<int>(0.5f)` | 60.957 | 60.601 |
| `std::gamma_distribution<float>(1.0f, 1.0f)` | 67.167 | 64.660 |
| `std::poisson_distribution<int>(1.0)` | 74.976 | 127.942 |
| `std::student_t_distribution<float>(3.0f)` | 105.936 | 140.108 |
| `std::bernoulli_distribution(0.5)` | 8.907 | 61.211 |
| `std::binomial_distribution<int>(10, 0.5)` | 354.444 | 607.976 |
| `std::exponential_distribution<float>(1.0f)` | 4.938 | 53.963 |
