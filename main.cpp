#include <iostream>
#include <chrono>
#include <assert.h>
#include <stdlib.h>
#include <random>
#include <algorithm>
#include <iterator>


int timeloop(int buffer_length, int iters) {
    std::vector<int> data(buffer_length, 0); //allocates buffer_length bytes of data
    // fill data with all 0s
    srand(time(NULL));
    for(int i = 0; i < buffer_length; i++) {
        data[i] = i;
    }
    //fill data with its address --> data[0] = 0, data[1] = 1
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(data.begin(), data.end(), g);
    //shuffle data - each element is now the address of a new data element
    std::chrono::time_point<std::chrono::high_resolution_clock> t1;
    std::chrono::time_point<std::chrono::high_resolution_clock> t2;
    int d0 = rand() % buffer_length;
    std::chrono::microseconds fastest_run = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::duration::max());
    for(int j = 0; j < iters; j++) {
        t1 = std::chrono::high_resolution_clock::now();
        for(int i = 0;i < buffer_length; i++){
            //fetch a single byte each time
            d0 = data[d0];
        }
        t2 = std::chrono::high_resolution_clock::now();
        if(std::chrono::duration_cast<std::chrono::microseconds>(t2-t1) < fastest_run) {
            fastest_run = std::chrono::duration_cast<std::chrono::microseconds>(t2-t1);
        }
    }
    std::cout <<buffer_length/1000 << "\t"<< 1000.0*(fastest_run.count()/static_cast<double>(buffer_length)) << std::endl;
    // std::cout <<buffer_length << "\t"<< 1000.0*(fastest_run.count()/static_cast<double>(buffer_length)) << "ns" << std::endl;
    return d0;

}
int main(int argc, char *argv[]) {
    assert(argc == 3);
    int buffer_length = atoi(argv[1]); //buffer size in bytes
    int iters = atoi(argv[2]);
    timeloop(buffer_length, iters);
    return 0;
}
