#include <iostream>
#include <iterator>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <queue>
#include <math.h>
#include <stdio.h>

using namespace std;

struct indices_data
{
	int thread_id;
	int start;
	int end;
};

extern vector<int> numbers;
extern struct timespec start_time, end_time;