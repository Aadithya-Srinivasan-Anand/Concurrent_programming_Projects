## LAB 2 -  Building concurrency primitives and the C/C++ memory model.

The two programs attached `main_sort` and `main_counter` verifies the implementation of locks and barriers. The counter program increments a common counter by certain number of iterations in every thread provided as input. where the `count` variable is placed at the critical section and the sort program sorts a file of integers concurrently using bucket sort and stores the sorted integers into a new output file. In both the programs, the concurrently primitives – lock or barrier are chosen based on the user provided input. The package includes a makefile which complies code and also cleans up the binaries.

# locks and Barriers

The implementation of locks is such that , an abstract class lock is created with two major methods, lock and unlock. the `locks_def.h` contains the defined user defined classes. Every sub class of the parent abstract class implements the lock and unlock method based on their respective algorithms – test and set, test and test and set, ticket and mcs locks are implemented. Further for the counters, each thread chooses the appropriate lock and locks before incrementing the counter and then unlocks after the increment. for bucket sort the implentation is build on top of Lab_1, where the array is divided into chunks equal to the number of threads. And each thread adds its elements into the bucket. An array of locks are chosen equal to the number of buckets for threads to access each bucket based on their lock. The barrier is implemented using a class sense is defined which handles the wait function of the barrier. Counter – for counter, every thread runs 𝑛𝑢𝑚𝑏𝑒𝑟 𝑡h𝑟𝑒𝑎𝑑𝑠 ∗ 𝑖𝑡𝑒𝑟𝑎𝑡𝑖𝑜𝑛𝑠 times and chooses to increment the counter based on the thread id. Once incremented, it waits for the other threads to also finish their first increment. Then all threads take turn for next round of increments.

# use case
`./mysort --name input.txt -o output.txt -t NUM_THREADS --bar=<sense,pthread> --lock=<tas,ttas,ticket,mcs,pthread>`

`./counter --name -t NUM_THREADS -i=NUM_ITERATIONS --bar=<sense,pthread> --lock=<tas,ttas,ticket,mcs,pthread> -o output.txt` 

(locks or barriers used one at a time for counter)

# implentation and perf analysis of all the locks and barriers implemented

for 550000in1-1000000skew.txt

**primitive**|**runtime**|**L1 cache hit**|**Branch pred hit rate**|**Page fault count**
:-----:|:-----:|:-----:|:-----:|:-----:
TAS|0.884114244|99.53%|99.34%|7,154
TTAS|0.903625841|99.49%|99.35%|7,155
Ticket|1.032007227|99.52%|99.38%|7,152
MCS|0.888016324|99.55%|99.33%|3,042
pthread|0.899834200|99.45%|99.35%|7,151  

for counter with 100 iterartions

**primitive**|**runtime**|**L1 cache hit**|**Branch pred hit rate**|**Page fault count**
:-----:|:-----:|:-----:|:-----:|:-----:
TAS|0.003334111|94.48%|97.29%|177
TTAS|0.003233701|95.89%|97.53%| 180
Ticket|0.002940526|97.88%|97.78%|177
MCS|0.003945977|98.51%|98.71%|150
pthread|0.022822126|95.00%|97.82%|179


**primitive**|**runtime**|**L1 cache hit**|**Branch pred hit rate**|**Page fault count**
:-----:|:-----:|:-----:|:-----:|:-----:
Bar - sense|0.004995161|98.93%|98.05%|148  
Bar - pthread|0.177213790|92.06%|97.89%|145  

# performance 

we can say that the test and set, test and test and set, locks works predomentently in similar fashion with these spin locks TTaS being a bit faster with teh execution time. 

Further, a code will benefit from using MCS lock when there's a high locks contention, i.e., many threads attempting to acquire a lock simultaneously. When using a simple spin lock, all threads are polling a single shared variable, whereas MCS forms a queue of waiting threads, such that each thread is polling on its predecessor in the queue. Hence cache coherency is much lighter since waiting is performed "locally".

 In mutex, waiting threads are usually queued by the OS and de-scheduled, so there's no polling whatsoever.
 
# extant bugs

Serever side error on L1 cache hit (rectified)

# Discription of code

`Locks.cpp` defines the locks that is being used in the sort and main functions.
`common_header.h` contains preprocessor directives and common header files
`locks_def.h` is header file with the class templates and the node for the locks to acessed, to create objects of class lock type.