

# Memory Management Simulator

A high-performance C++ simulator that models Operating System memory management. This project simulates physical memory allocation using **Variable Partitioning** and the **Buddy System**, integrated with a **Multilevel Set-Associative Cache** (L1 & L2). It provides detailed metrics on fragmentation and cache performance using a **1:1 physical address implementation**.

## Key Features

*   **Standard Allocator (Variable Partitioning):** 
    *   Supports **First Fit**, **Best Fit**, and **Worst Fit** strategies.
    *   Implements **Greedy Coalescing** to merge adjacent free holes.
*   **Buddy Allocator (Fixed Partitioning):** 
    *   Enforces power-of-two ($2^n$) block sizing.
    *   Uses **Bitwise XOR math** for $O(1)$ buddy lookup and merging.
*   **Multilevel Cache Hierarchy:**
    *   Simulates L1 and L2 caches with **FIFO (First-In, First-Out)** replacement.
    *   Configurable associativity, allowing for **Direct-Mapped** to **N-Way** set simulation.
*   **Performance Metrics:** Real-time tracking of Internal/External fragmentation and Cache Hit Ratios.

---

## Folder Structure

Based on the repository layout:

```text
.
├── src/                    # All implementation source files
│   ├── allocator/          # Logic for Variable Partitioning (Standard Mode)
│   ├── buddy/              # Logic for the Power-of-Two Buddy System
│   ├── cache/              # Multilevel Cache and FIFO logic
│   └── main.cpp            # The CLI and System Integration
├── tests/                  # Test artifacts and workloads
│   ├── demonstration_tests/# Logs specifically from the demo video
│   └── randomgenerator/    # Workloads generated for stress testing
├── Makefile                # Automated build script
├── DESIGNDOC.pdf           # Detailed technical design and diagrams
├── testingscript.sh        # Automated artifact and log generator
├── DEMONSTRATION_MATERIAL_LINK # Link to the project demo video
└── .gitignore              # Standard ignore rules
```

---

## Local Setup & Building

### Prerequisites
*   A C++ compiler (GCC/G++ recommended).
*   `make` utility.

### Build Instructions
1.  **Clone the Repository:**
    ```bash
    git clone https://github.com/iitiankrrish/MemoryManagementSimulator.git
    cd memory-simulator
    ```
2.  **Compile the Project:**
    ```bash
    make
    ```
3.  **Run the Simulator:**
    ```bash
    ./memsim
    ```

---

## Usage Guide

Once running, the simulator provides an interactive prompt `> `.

### Initializing Memory
*   **Standard Mode:** `init memory 1024` (Creates 1024 bytes of variable memory).
*   **Buddy Mode:** `init buddy 1024 16` (Creates 1024 bytes with a 16-byte minimum block size).

### Allocation & Stats
*   `malloc <size>`: Allocates memory and returns an **ID** (Standard) or **Address** (Buddy).
*   `free <id/address>`: Releases the block and triggers coalescing.
*   `stats`: Displays fragmentation, utilization, and success rates.
*   `dump`: Visualizes the current state of every memory block or buddy level.

### Cache Operations
*   `cache init <L1/L2> <size> <block_size> <assoc>`: Configures a cache level.
*   `cache access <0xaddr>`: Simulates a CPU memory access.
*   `cache stats`: Shows hit/miss counts and hit rates.

---

## Project Nuances

### 1. The ID vs. Address Distinction
In **Standard Mode**, the user interacts with **Block IDs**. In **Buddy Mode**, the user interacts with **Hexadecimal Addresses**. This reflects the reality that Buddy systems are low-level physical managers, while variable partitioning often acts as a higher-level heap manager.

### 2. Conflict Misses (Cache Thrashing)
When the cache is configured as **Direct-Mapped (Associativity = 1)**, multiple memory addresses may map to the same set. The simulator accurately models **Conflict Misses**, where two frequently used addresses repeatedly evict each other.

---

## Testing

To verify the project's robustness, you can run the automated test suite:
1.  Run `./testingscript.sh`.
2.  Check the `tests/` folder for generated `.txt` (inputs) and `.log` (outputs).

---
## Demonstration Material

[Demonstration Video](https://drive.google.com/file/d/1nNCsrn0-13jIaGHpA1Gy5r3ToWzSXLo6/view?usp=sharing)

