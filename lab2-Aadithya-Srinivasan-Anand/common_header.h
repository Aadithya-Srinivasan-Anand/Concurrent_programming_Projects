#include <iostream>
#include <iterator>
#include <vector>
#include <mutex>
#include <queue>
#include <math.h>
#include <stdio.h>
#include <atomic>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define RELAXED std::memory_order_relaxed
#define RELEASE std::memory_order_release
#define SEQ_CST std::memory_order_seq_cst
#define ACQUIRE std::memory_order_acquire
