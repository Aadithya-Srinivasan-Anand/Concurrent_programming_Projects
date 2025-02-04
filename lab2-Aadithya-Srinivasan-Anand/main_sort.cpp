// ./mysort --name input.txt -o output.txt -t NUM_THREADS --bar=<sense,pthread> --lock=<tas,ttas,ticket,mcs,pthread>

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <getopt.h>
#include <pthread.h>
#include <iterator>
#include <stdlib.h>
#include <unistd.h>
#include <atomic>
#include <cstdlib>
#include <sys/types.h>
#include <sys/syscall.h>


#include "locks_def.h"


using namespace std;

struct indices_data     // initialising indices inside the main sort rather than in separte header, unlike lab_1
{
	int thread_id;
	int start;
	int end;
};

//global init

int NUM_THREADS;         //input from the user on the number of threads
vector<int> numbers;     // vectors of numbers to hold
int num_of_buckets;
int maximum;
int chunk_size;
struct timespec start_time, end_time;

vector<priority_queue<int, vector<int>, greater<int>>> all_buckets;


//Locks

bool is_lock = false;
vector<locks*> lk;      // initialising a vector of lock type, defined in the lock_def.h
vector<mutex> mtx;      // vector of mutex, predefined primitive

bool mcs_flag = false;
bool pthread_flag = false;


//Barrier

bool is_barrier = false;
barrier *sense_barrier;
bool pthread_bar_flag = false;
pthread_barrier_t pthread_bar;


// read input

vector<int> read_input(string input_file_name)  // fuction to read a file of integers from a file
{
    ifstream read(input_file_name);             // read the file name 
    string n;
    vector<int> numbers;                        // store the contents as a vector of numbers
    while (read >> n){
        numbers.push_back(stoi( n ));
    }
    return numbers;                             // return the numbers stored (vector)
}

// write input to file

void write_to_file(vector<int> numbers, string output_file_name)
{
    ofstream output_file(output_file_name);         
    ostream_iterator<signed long long int> output_iterator(output_file, "\n");
    copy(numbers.begin(), numbers.end(), output_iterator);
}


// Display elapsed time

void show_elapsed_time()
{
    unsigned long long elapsed_ns;  
	elapsed_ns = (end_time.tv_sec-start_time.tv_sec)*1000000000 + (end_time.tv_nsec-start_time.tv_nsec);
	printf("Elapsed (ns): %llu\n",elapsed_ns);
	double elapsed_s = ((double)elapsed_ns)/1000000000.0;
	printf("Elapsed (s): %lf\n",elapsed_s);
}


// bucket sort

void *bucket_sort(void *input)              
{
    // local initialisation for indices_data

	struct indices_data *data = (struct indices_data*)input;
    int start = data->start;
    int end = data->end;
    int tid = data->thread_id;

    if(tid==0)      // bucket sort algorithm
    {
		clock_gettime(CLOCK_MONOTONIC,&start_time);
	}
    for(int i = start; i <= end; i++ )
    {
        int bucket_number = floor(num_of_buckets * numbers[i] / maximum);
        if(bucket_number >= num_of_buckets)
        {
            bucket_number = num_of_buckets - 1;
        }

        node *thread_node = new node();     // creation of the thread node object from node class defined in the header

        // Lock and unlock using the appropriate user choice
        if(mcs_flag)
        {
            lk[bucket_number]->lock(thread_node);   
        }else if(pthread_flag)
        {
            mtx[bucket_number].lock();
        }else
        {
            lk[bucket_number]->lock();
        }

        all_buckets[bucket_number].push(numbers[i]);

        if(mcs_flag)
        {
            lk[bucket_number]->unlock(thread_node);
        }else if(pthread_flag)
        {
            mtx[bucket_number].unlock();
        }else
        {
            lk[bucket_number]->unlock();
        }

    }

	if(tid==0)
    {
		clock_gettime(CLOCK_MONOTONIC,&end_time);
	}
    return NULL;
}


void *bucket_sort_barrier(void *input)
{
    // local initialisation of indices_data

	struct indices_data *data = (struct indices_data*)input;
    int start = data->start;
    int end = data->end;
    int tid = data->thread_id;
    

    if(tid==0)
    {
		clock_gettime(CLOCK_MONOTONIC,&start_time);
	}

    int size = (end - start) + 1;

    for(int i = 0; i < size * NUM_THREADS; i++)
    {
        if(i % NUM_THREADS == tid)
        {
            int bucket_number = floor(num_of_buckets * numbers[start] / maximum);
            if(bucket_number >= num_of_buckets)
            {
                bucket_number = num_of_buckets - 1;
            }

            node *thread_node = new node();
            
            if(is_lock)
            {
                // Lock and unlock using the appropriate user choice
                if(mcs_flag)
                {
                    lk[bucket_number]->lock(thread_node);
                }else if(pthread_flag)
                {
                    mtx[bucket_number].lock();
                }else
                {
                    lk[bucket_number]->lock();
                }
            }

            all_buckets[bucket_number].push(numbers[start]);

            if(is_lock)
            {
                if(mcs_flag)
                {
                    lk[bucket_number]->unlock(thread_node);
                }else if(pthread_flag)
                {
                    mtx[bucket_number].unlock();
                }else
                {
                    lk[bucket_number]->unlock();
                }
            }

            start += 1;
        }
        // Using barrier to sync across multiple threads
        if(pthread_bar_flag == true)
        {
            pthread_barrier_wait(&pthread_bar);
        }else
        {
            sense_barrier->wait();
        }
    }

	if(tid==0)
    {
		clock_gettime(CLOCK_MONOTONIC,&end_time);
	}
    return NULL;
}

//parallelization part

void multithreaded_bucket_sort(int num_threads, int chunk_size)
{
    // global initialisation, thread init

	pthread_t* threads = (pthread_t*)malloc((num_threads)*sizeof(pthread_t));
	size_t current_chunk = chunk_size;

	struct indices_data data[num_threads];
    data[0].thread_id = 0;
    data[0].start = 0;
    data[0].end = chunk_size-1;


	for(int i = 1; i < num_threads; i++)
    {
		int start = current_chunk;
        int end;
        if(current_chunk + chunk_size >= numbers.size())
        {
            end = numbers.size() - 1;
        }else
        {
            end = current_chunk + chunk_size - 1;
        }
        data[i].thread_id = i;
        data[i].start = start;
        data[i].end = end;
        current_chunk += chunk_size;
	}


    // create threads
	for(int i = 1; i < num_threads; i++)
    {
        int ret;
        if(is_barrier)
        {
            ret = pthread_create(&(threads[i]), NULL, &bucket_sort_barrier, (void *)(&data[i]));
        }else
        {
            ret = pthread_create(&(threads[i]), NULL, &bucket_sort, (void *)(&data[i]));
        }
        if(ret)
        {
            printf("ERROR; pthread_create: %d\n", ret);
            exit(-1);
        }
    }
    if(is_barrier)
    {
        bucket_sort_barrier((void *)(&data[0]));
    }else
    {
        bucket_sort((void *)(&data[0]));
    }

    // join threads
    for(int i=1; i < num_threads; i++)
    {
        int ret = pthread_join(threads[i],NULL);
        if(ret)
        {
            printf("ERROR; pthread_join: %d\n", ret);
            exit(-1);
        }
    }

    numbers.clear();
    for(int i = 0; i < num_of_buckets; i++)
    {
        while(!all_buckets[i].empty())
        {
            numbers.push_back(all_buckets[i].top());
            all_buckets[i].pop();
        }
    }

    // global clean up
    free(threads);
}



//Read numbers from file, sort based on input algorithm and write output to a file

int main(int argc, char *argv[])
{
    static struct option long_options[] = 
    {
        {"name", no_argument, NULL, 'n'},
        {"bar", required_argument, NULL, 'b'},
        {"lock", required_argument, NULL, 'l'},
        {0, 0, 0, 0}
    };

    //option variables

    bool name = false;
    string input_file_name = "";
    string output_file_name = "";
    int num_threads = 1;
    string bar = "";
    string lock = "";

    while (true) 
    {
        int option_index = 0;
        int flag_char = getopt_long(argc, argv, "o:t:", long_options, &option_index);
        if (flag_char == -1)
        {
          break;
        }

        switch (flag_char)
        {
            case 'n':
                name = true;
                break;
            case 'b':
                bar = std::string(optarg);
                break;
            case 'l':
                lock = std::string(optarg);
                break;
            case 'o':
                output_file_name = std::string(optarg);
                break;
            case 't':
                num_threads = stoi(optarg);
                NUM_THREADS = num_threads;
                break;
            case '?':
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }

    if (optind < argc)
    {
        input_file_name = std::string(argv[optind]);
    }

    if(name)
    {
        cout << "Aadithya Srinivasan Anand\n";
    }

	if(argc > 2)
    {
	    if(input_file_name == "" || output_file_name == "" || ( bar == "" && lock == ""))
        {
            cout<<"invalid";
        }else
        {
	        numbers = read_input(input_file_name);

            // divide array into chunks, each thread will get a chunk
            chunk_size = ceil(float(numbers.size()) / num_threads);

                if(bar != "")
                {
                    is_barrier = true;
                }

                int count = std::abs(int(numbers.size() - (chunk_size * num_threads)));
                if(is_barrier && count > 0)
                {
                    numbers.insert(numbers.end(), count, -1);
                }

                num_of_buckets = ceil(float(log(numbers.size())));
                maximum = *std::max_element(numbers.begin(), numbers.end());
                all_buckets.resize(num_of_buckets);

                if(bar != "")
                {
                    is_barrier = true;
                    if(bar == "sense")
                    {
                        sense_barrier = new barrier(num_threads-1);
                    }else if (bar == "pthread")
                    {
                        pthread_bar_flag = true;
                        pthread_barrier_init(&pthread_bar, NULL, num_threads);
                    }
                }
                
                if(lock != ""){
                    is_lock = true;
                    if (lock == "tas")
                    {
                        lk.insert(lk.end(), num_of_buckets, new test_and_set());
                    }else if (lock == "ttas")
                    {
                        lk.insert(lk.end(), num_of_buckets, new test_and_test_and_set());
                    }else if (lock == "ticket")
                    {
                        lk.insert(lk.end(), num_of_buckets, new ticket_lock());
                    }else if (lock == "mcs")
                    {
                        mcs_flag = true;
                        lk.insert(lk.end(), num_of_buckets, new mcs());
                    }else if (lock == "pthread")
                    {
                        pthread_flag = true;
                        std::vector<std::mutex> list(num_of_buckets);
                        mtx.swap(list);
                    }else
                    {
                        cout<<"invalid";
                    }
                }
                multithreaded_bucket_sort(num_threads, chunk_size);

                if(bar == "pthread")
                {
                    pthread_barrier_destroy(&pthread_bar);
                }

                if(is_barrier && count > 0)
                {
                    numbers.erase(numbers.begin(), numbers.begin() + count);
                }
	        write_to_file(numbers, output_file_name);
            show_elapsed_time();
	    }
    }
    return 0;
}