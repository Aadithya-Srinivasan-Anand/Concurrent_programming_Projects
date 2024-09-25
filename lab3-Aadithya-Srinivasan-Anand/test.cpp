#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <getopt.h>
#include <iterator>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

using namespace std;

vector<int> numbers;
struct timespec start_time, end_time;

vector<int> read_input(string input_file_name) // reading a file of integers
{
    ifstream read(input_file_name);
    string n;
    vector<int> numbers;
    while (read >> n){
        numbers.push_back(stoi( n ));
    }
    return numbers;
}

void write_to_file(vector<int> numbers, string output_file_name) // writing a vector of integers into a file
{
    ofstream output_file(output_file_name);
    ostream_iterator<signed long long int> output_iterator(output_file, "\n");
    copy(numbers.begin(), numbers.end(), output_iterator);
}

int partition(vector<int> &numbers, int start, int end) //breakdown and to get the partition index of the vector
{
    int pivot = numbers[end];
    int partition_index = start;

    int temp;

    for(int i = start; i < end; i++)
    {
        if(numbers[i] <= pivot)
        {
            temp = numbers[i];
            numbers[i] = numbers[partition_index];
            numbers[partition_index] = temp;
            partition_index++;
        }
    }

    temp = numbers[end];
    numbers[end] = numbers[partition_index];
    numbers[partition_index] = temp;

    return partition_index;
}

void openmp_quick_sort(vector<int> &numbers, int start, int end) // quicksort through OpenMP multi threading
{
    int size  = numbers.size();
    if(end >= size)
    {
        end = size - 1;
    }
    if(start < end)
    {
        int partition_index = partition(numbers, start, end);

        openmp_quick_sort(numbers, start, partition_index-1);
        
        openmp_quick_sort(numbers, partition_index+1, end);
    }
}

void combine_two(int start, int mid, int end) // Combining 2 sorted arrays into one array
{
    int size  = numbers.size();
    if(start >= size)
    {
        return;
    }
    if(mid >= size)
    {
        mid = size-1;
    }
    if(end >= size)
    {
        end = size-1;
    }

    vector<int> list_1;
    vector<int> list_2;

    for(int i = start; i <= mid; i++)
    {
        list_1.push_back(numbers[i]);
    }
    for(int i = mid+1; i <= end; i++)
    {
        list_2.push_back(numbers[i]);
    }

    size_t current = start, i = 0, j = 0;

    while(i < list_1.size() and j < list_2.size())
    {
        if(list_1[i]<=list_2[j])
        {
            numbers[current] = list_1[i];
            i++;
        } else 
        {
            numbers[current] = list_2[j];
            j++;
        }
        current++;
    }

    while(i < list_1.size()) 
    {
        numbers[current] = list_1[i];
        current++;
        i++;
    }

    while(j < list_2.size()) 
    {
        numbers[current] = list_2[j];
        current++;
        j++;
    }
}

void combine_k(int chunk_size, int num_threads) // Given K arrays which are sorted, combine them into one using OpenMP
{
    int times = 0;
    int size  = numbers.size();

    while(times++ <= log(num_threads) + 1)
    {
        #pragma omp for
        for(int start = 0; start < size; start = start + (2 * chunk_size))
        {
            combine_two(start, start + chunk_size - 1, start + (2 * chunk_size) - 1);
        }

        chunk_size *= 2;
    }
}

int main(int argc, char *argv[])
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
        numbers = read_input(input_file_name);
        omp_set_num_threads(num_threads);

        // divide array into chunks, each thread will get a chunk
        int chunk_size = ceil(float(numbers.size()) / omp_get_max_threads());
        int size  = numbers.size();
        if(algo == "merge")
        {
            exit(1);
        }
        else if(algo == "quick" || algo =="") // only quicksort implemented
        {
        clock_gettime(CLOCK_MONOTONIC,&start_time);
        #pragma omp parallel default(none) shared(numbers, chunk_size, size)
        {
            #pragma omp for
            for(int start = 0; start < size; start = start + chunk_size){
                openmp_quick_sort(numbers, start, start + chunk_size - 1);
            }
            if(omp_get_max_threads() > 1)
            {
                combine_k(chunk_size, omp_get_max_threads());
            }
        }
        }    
        clock_gettime(CLOCK_MONOTONIC,&end_time);
        write_to_file(numbers, output_file_name);
    }
    return 0;
}
