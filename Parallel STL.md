# Parallel STL

The Standard Template Library has more than 100 algorithms for searching, counting, and manipulating ranges and their elements. With C++17, 69 of them are overloaded and 8 new ones are added. The overloaded and new algorithms can be invoked with a so-called execution policy.

## Execution Policy

The policy tag specifies whether an algorithm should run sequentially, in parallel, or in parallel with vectorization.

* `std::parallel::seq`: runs the algorithm sequentially
* `std::parallel::par`: runs the algorithm in parallel on multiple threads
* `std::parallel::par_unseq`: runs the algorithm in parallel on multiple threads and allows the interleaving of individual loops; this permits the usage of a vectorized version with [SIMD](https://en.wikipedia.org/wiki/SIMD) (**S**ingle **I**nstruction **M**ultiple **D**ata) extensions.

```cpp
// standard sequential sort
std::sort(v.begin(), v.end());

// sequential execution
std::sort(std::parallel::seq, v.begin(), v.end());

// permitting parallel execution
std::sort(std::parallel::par, v.begin(), v.end());

// permitting parallel and vectorized execution
std::sort(std::parallel::par_unseq, v.begin(), v.end());
```

> The parallel algorithm does not automatically protect you from data races and deadlocks.


