# Memory Hierarchy benchmarks

### Part 1: Coding
#### `timer.sh`
This script calls my C++ code (`main.cpp`) 17 times, with the argument ranging from 2^12 to 2^26 (increasing by powers of two each time) and a constant 10 iterations. The C++ code is responsible for printing output, so the script is very simple.

#### `main.cpp`
This program reads two inputs from the command line: `buffer_size` and `iters`. It creates a new vector of `buffer_length` integers. Since an int is 4 bytes, I allocate `buffer_size/sizeof(int)` total integers. Allocating fewer than 2^12 bytes of memory caused incoherent results, since the machine cannot accurately time reads of less than 1 nanosecond.

Each value is initialized as 0, but is then written over as it's position in the array - data[0] is 0, data[1] is 1, and so on. I use a standard library function to derange the values, so the values are now in a random order.

I then read every byte in the array in that random order. By using the contents of the byte as the next address to read, the compiler cannot precompute the order. I return the final value to require that the value is computed, while remaining compliant with POSIX exit code conventions - returning 1 from `main` is used to indicate a failure.

Although timing the entire loop also adds the overhead of timing the initialization, comparison and increment of the index variable, this adds a relatively negligible amount of overhead. The goal of this project is to determine the sizes of my caches, so the actual speed is unimportant. The overhead associated with initializing the index variable is constant, and the overhead associated with the loop is linear in the iterations, so the effect is essentially the same for all sizes.

This file was compiled using `gcc version 7.4.0 (Ubuntu 7.4.0-1ubuntu1~18.04.1)`

To build this file, I used `g++ -O3 -o bin.out main.cpp`

### Part 2: graph
![Avg Runtime vs Buffer size](../master/graph.png)

This graph was generated with the command `gnuplot make.plot`, which uses the data in `results.dat` as input. My C++ code outputs the buffer size in KB (using integer division for legibility) and the fastest average time to read per byte in nanoseconds as a tab separated value. Piping the output (or copying it from the terminal) into a text file creates input that gnuplot can understand.

Using the linked values, I calculated the expected average time as described in Part 3, these results are shown as `expected.dat`. Both values are plotted below.

![Expected, Measured Avg Runtimes](../master/graph2.png)

Because each buffer size is a power of 2 larger, I used a logscale on the x axis so that each data point is evenly spaced. Using a large (1980x1080) image makes the graph more readable.

### Part 3: Analysis
If the buffer size is below the size of the L1 cache, we expect the entire buffer to be prefetched into the cache (thanks to the g++ builtin prefetch), giving a hit rate of 100%. If the buffer is twice the size of the L1 cache, we expect the L1 hit rate to be 50%.

Using the examples given during lecture, we would expect the average time for an L1 hit rate of 50% and L2 hit rate of 100% to be
$(0.5)(1) + (1-0.5)(3)$ or 2ns.

However, by querying the machine directly, we find that [`cat /sys/devices/system/cpu/cpu0/cache/index1/size`](https://stackoverflow.com/questions/20330509/different-cpu-cache-size-reported-by-sys-device-and-dmidecode) tells us that my VM has 32KB of L1 cache, 256KB of L2 cache and 8192KB of L3 cache. Looking up the processor using native tools, I find the same result.

With 32KB of L1 cache, we should expect that all buffers less than 32KB to be roughly the same average time. Looking at the graph, we see a mostly flat region, and the 65KB buffer takes longer (2.6ns) than the smaller sizes, which range from slightly less than 1ns to 1.58 ns, although the reason for the positive slope in that region is unclear to me.

Using 524KB of data, both the L1 and L2 caches should be full. Since only 32KB of data fit in the L1 cache and 256KB of data in the L2 cache, we expect the hit rates to be 32/524(6%) and 256/524(48%) respectively.

#### Expected vs measured
Qualitatively, the graphs have roughly the same shape - as the L2 hit rate drops, the average time increases, with a drastic increase once the L3 cache is filled. The spike in average runtime from a 4MB to an 8MB buffer exists because the 4MB buffer is, like the other small buffers, much faster than the listed values predict. For large (64MB) buffers, we measured values worse than expected, possibly due to the way the VM handles cache access vs DRAM access. Nonetheless, the similarity in the shape in the graphs indicates that the experiment is methodologically sound.

Consulting [more recent sources](https://stackoverflow.com/questions/4087280/approximate-cost-to-access-various-caches-and-main-memory) gives different values for the relative times to read from each cache. I plotted these values as `source2` on the second graph, and they match much more closely. I increased the readtime for DRAM to 150ns and created a second expectation, plotted as `expected2`. This matches my measured values almost exactly, indicating that the use of a VM just more than doubles my DRAM read times. 
