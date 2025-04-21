# A Bitwise-Compatible Approach to SCER Matching in Indeterminate Strings

This repository provides the official implementation of the algorithms proposed in our paper:

> **A Bitwise-Compatible Approach to SCER Matching in Indeterminate Strings**  
> *Simone Faro, Dominik Köppl, Thierry Lecroq, Francesco Pio Marino*  

## 📄 Overview

This work addresses the problem of matching a **determinate pattern** against an **indeterminate text**, where each text position may represent a set of possible characters. We focus on two prominent substring-consistent equivalence relations (SCERs):

- **Order-Preserving Matching**
- **Parameterized Matching**

Our main contribution is a set of **bitwise-compatible** algorithms that:
- Avoid explicit set operations.
- Use **sum-free sequences** (powers of two) for encoding.
- Support constant-time character comparisons via bitwise operations.
- Achieve optimal or near-optimal performance in both theory and practice.

## ⚡ Performance

The algorithms are:
- **Order-Preserving Matching**: `O(n)` time, in-place, with constant-time operations.
- **Parameterized Matching**: `O(n + σ_p √(σ_p^x σ_p^y))` time via reduction to bipartite matching.

## 🛠 Code Structure

- `oppm.c`: Bit-parallel algorithm for Order-Preserving Pattern Matching.
- `pism.c`: Algorithm for Parameterized Matching with indeterminate text.

The parameterized matcher reduces injectivity enforcement to **maximum bipartite matching**, solved using the **Hopcroft-Karp algorithm**.

### 📌 Attribution

The Hopcroft-Karp implementation is adapted from:

> https://github.com/HackersUOP/codeBase/blob/master/codeBase/Algorithms/Matching_Algorithms/Maximum_Matching-Bipartite_Graphs/Hopcroft-Karp_Algorithm/hopcroft-karp.cpp

Credit to the original authors is preserved in the code headers.
