/**
	Program to perform Time_Count testing for low contention on the skip list.
    ./Time_Count [--name] -m <max_number> -t <num_threads> 
*/
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <iterator>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "skip_list.h"

using namespace std;

size_t num_threads = 1;
Skip_List skiplist;
size_t max_number = 100;
struct timespec start_time, end_time;

/**
    Integers to be used for operations
*/
vector<int> numbers_insert;
vector<int> numbers_delete;
vector<int> numbers_get;
vector<pair<int,int>> numbers_range;

/**
    Display elapsed time
*/
void Total_time_taken(){
    unsigned long long elapsed_ns;
	elapsed_ns = (end_time.tv_sec-start_time.tv_sec)*1000000000 + (end_time.tv_nsec-start_time.tv_nsec);
	printf("Elapsed (ns): %llu\n",elapsed_ns);
	double elapsed_s = ((double)elapsed_ns)/1000000000.0;
	printf("Elapsed (s): %lf\n",elapsed_s);
}

void generate_input(int max_number){
    // generating insert data
    for(int i = 1; i <= max_number; i++){
        numbers_insert.push_back(i);
    }

    // generating delete data
    for(size_t i = 0; i < numbers_insert.size(); i++){
        numbers_delete.push_back(numbers_insert[i]);
    }

    // generating search data
    for(size_t i = 0; i < numbers_insert.size(); i++){
        numbers_get.push_back(numbers_insert[i]);
    }

    // generating range data
    for(size_t i = 0; i < num_threads; i++){
        int a = (rand() % static_cast<int>(max_number + 1));
        int b = a + (rand() % static_cast<int>(max_number - a + 1));
        numbers_range.push_back(make_pair(a,b));
    }
}

void skiplist_add(size_t start, size_t end)
{
    if(end >= numbers_insert.size()) end = numbers_insert.size();
    if(start == end) skiplist.add(numbers_insert[start], to_string(numbers_insert[start]));
    for(size_t i = start; i < end; i++){
        skiplist.add(numbers_insert[i], to_string(numbers_insert[i]));
    }
}

void low_contention_Time_Count()
{

    vector<thread> threads;

    // generating insert data
    for(size_t i = 1; i <= max_number; i++){
        numbers_insert.push_back(i);
    }

    skiplist = Skip_List(numbers_insert.size(), 0.3);

    // insert
    int chunk_size = ceil(float(numbers_insert.size()) / num_threads);
    for(size_t i = 0; i < numbers_insert.size(); i = i + chunk_size){
        threads.push_back(thread(skiplist_add, i, i+chunk_size));
    }
    for (auto &th : threads) {
        th.join();
    }

    threads.clear();
}


/**
    Performs the insert, delete, get and range opetations on the skiplist to Time_Count test it.
*/
int main(int argc, char *argv[]){

    static struct option long_options[] = {
        {"name", no_argument, NULL, 'n'},
        {0, 0, 0, 0}
    };

    //Variables for options
    bool name = false;
    string Time_Count = "";

    while (true) {
        int option_index = 0;
        int flag_char = getopt_long(argc, argv, "m:t:", long_options, &option_index);
        if (flag_char == -1) {
          break;
        }

        switch (flag_char) {
            case 'n':
                name = true;
                break;
            case 't':
                num_threads = stoi(optarg);
                break;
            case 'm':
                max_number = stoi(optarg);
                break;
            case '?':
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }

   if(name){
        cout << "\nAadithya Srinivasan Anand\n";
    }

	else if(argc > 2)
    {
	    if(max_number <= 0 || num_threads < 1 )
        {

	    }
	    else
        {
                clock_gettime(CLOCK_MONOTONIC,&start_time);
                low_contention_Time_Count();
                clock_gettime(CLOCK_MONOTONIC,&end_time);
	    }
        Total_time_taken();
	    
    }
    return 0;
}