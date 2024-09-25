#include <iostream>
#include <string>
#include <fstream>
#include <iterator>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <getopt.h>
#include <pthread.h>
#include "common_head.h"
#include "quick_sort.h"
#include "bucket_sort.h"
using namespace std;

vector<int> numbers;
struct timespec start_time, end_time;

vector<int> read_input_file(string input_file_name) // string input_file_name is the input to be read.
{
    ifstream read(input_file_name);
    string n;
    vector<int> numbers;
    while (read >> n)
    {
        numbers.push_back(stoi( n ));
    }
    return numbers; // return the numbers from the file.
}

void write_into_file(vector<int> numbers, string output_file_name) // writing the vector integers into a file
{
    ofstream output_file(output_file_name);
    ostream_iterator<signed long long int> output_iterator(output_file, "\n");
    copy(numbers.begin(), numbers.end(), output_iterator);
}

void show_time() // computing time.
{
    unsigned long long elapsed_ns; 
	elapsed_ns = (end_time.tv_sec-start_time.tv_sec)*1000000000 + (end_time.tv_nsec-start_time.tv_nsec);
	printf("Elapsed (ns): %llu\n",elapsed_ns);
	double elapsed_s = ((double)elapsed_ns)/1000000000.0;
	printf("Elapsed (s): %lf\n",elapsed_s);
}

int main(int argc, char *argv[]) // parsing command line
{

    static struct option long_options[] = {
        {"name", no_argument, NULL, 'n'},
        {"alg", required_argument, NULL, 'a'},
        {0, 0, 0, 0}
    };

    //Variables for options
    bool name = false;
    string input_file_name = "";
    string output_file_name = "";
    int num_threads = 1;
    string algo = "";

    while (true) {
        int option_index = 0;
        int flag_char = getopt_long(argc, argv, "o:t:", long_options, &option_index);
        if (flag_char == -1) {
          break;
        }

        switch (flag_char) {
            case 'n':
                name = true;
                break;
            case 'a':
                algo = std::string(optarg);
                break;
            case 'o':
                output_file_name = std::string(optarg);
                break;
            case 't':
                num_threads = stoi(optarg);
                break;
            case '?':
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }

    if (optind < argc) {
        input_file_name = std::string(argv[optind]);
    }

    if(name){
        cout << "Aadithya Srinivasan Anand\n";
    }

	if(argc > 2)
    {
	    numbers = read_input_file(input_file_name);
        int block_size = ceil(float(numbers.size()) / num_threads); // dividing the arry into separate blocks, that the thread can operate on
	    if(algo == "forkjoin") // calling the sort options
        {
	        parallel_quick_sort(num_threads, block_size);
	    }
	    else if (algo == "lkbucket")
        {
            parallel_bucket_sort(num_threads, block_size);
	    }else
        {
	        cout << "Invalid algorithm type";
	    }
	    write_into_file(numbers, output_file_name);
        show_time();

    }
    return 0;
}