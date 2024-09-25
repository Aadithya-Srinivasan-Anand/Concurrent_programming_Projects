
# Concurrent Project - Unordered maps - Skiplist

A skip list is probabilistic data structure with a logarithmic insertion, deletion and search in the average case with an ordered sequence of elements while maintaining a linked list like structure. The skip list is built on multiple layers. The bottom layer has all the elements connected and the topper layers behave as an express lane to skip few elements to transverse the skip list faster. Insert, search, delete all traverse the express lane to find the right position quickly and then perform the necessary action.

## Code organisation

The code is organized into different files and modules based on their functionality. Code is written
such that each structure has different cpp file and header file.

• The `main.cpp` file handles the reading of command line arguments, initializing and using
the slip list based on user input.

• The project also contains 2 unit test files which create sample skip list and run operations
on it.

• The `High_content` and `Low_content` file is used to test the skip list for time taken for High and low contention.

The code also conatins `headers.h` and `headers.cpp` contains the keyvalue pair and also the node class definition. The `skip_list` class file handles all the operations of the skip list.
## Performance

High contention

In order to simulate high contention, the High_content code executes such that it continuously tries
to add the same element and delete the same element for certain number of iterations. Doing this
make all the threads try to acquire the lock of the element to be deleted and its predecessor. The
results of time taken, and page faults are tabulated below

**Elements**|**Threads**|**Time (s)**|**page faults**
:-----:|:-----:|:-----:|:-----:
100|4|0.016224|799
1000|4|0.592064|2003

Low contention

In order to simulate low contention, the Low_content code executes such that it inserts to the skip
list a wider range of values such that the multiple inserts do not try to acquire lock of the same
nodes but try to uniformly acquire locks. This is simulated by inserting all values from 1 to
num_iterations so that there is uniform insert and holding locks or contention for resources also
takes place uniformly. But, it is to be noted that the higher the level a node is available the higher
the chance of threads being contending for it since threads try to acquire lock of element if they
are a predecessor in higher levels. The results of time taken, and page faults are tabulated below

**Elements**|**Threads**|**Time (s)**|**page faults**
:-----:|:-----:|:-----:|:-----:
100|4|0.000937|56
1000|4|0.003199|189

## Execution Instruction

`./skiplist [--name] -m <iterations> -t <num_threads>`
`./High_content [--name] -m <iterations> -t <num_threads>`
`./Low_content [--name] -m <iterations> -t <num_threads>`

## extant bugs

INT_MAX val detrmination. 
seg fault rectification.
