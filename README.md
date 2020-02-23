# Memory Hierarchy benchmarks

### Part 1: Coding
#### `timer.sh`
This script calls my C++ code (`main.cpp`) 17 times, with the argument ranging from 2^10 to 2^26 (increasing by powers of two each time) and a constant 10 iterations. The C++ code is responsible for printing output, so the script is very simple.

#### `main.cpp`
This program reads two inputs from the command line: `buffer_length` and `iters`. It creates a new vector of `buffer_length` integers. Since an int is 4 bytes, calling this with an argument of 2^10 allocates 4*2^10 = 2^12 bytes of memory. Allocating fewer than 2^10 bytes of memory caused incoherent results, since the machine cannot accurately time reads of less than 1ns.

Each value is initialized as 0, but is then written over as it's position in the array - data[0] is 0, data[1] is 1, and so on. I use a standard library function to derange the values, so the values are now in a random order.

I then read every byte in the array in that random order. By using the contents of the byte as the next address to read, the compiler cannot precompute the order. I return the final value to require that the value is computed, while remaining compliant with POSIX exit code conventions - returning 1 from `main` is used to indicate a failure.

Although timing the entire loop also adds the overhead of timing the initialization, comparison and increment of the index variable, this adds a relatively negligible amount of overhead. The goal of this project is to determine the sizes of my caches, so the actual speed is unimportant. The overhead associated with initializing the index variable is constant, and the overhead associated with the loop is linear in the iterations, so the effect is essentially the same for all sizes.

This file was compiled using `gcc version 7.4.0 (Ubuntu 7.4.0-1ubuntu1~18.04.1)`

To build this file, I used `g++ -O3 -o bin.out main.cpp`

### Part 2: graph
![Graph](https://github.com/jon-takagi/CS389HW1/blob/master/graph.png)

This graph shows 
### Part 3: Analysis
