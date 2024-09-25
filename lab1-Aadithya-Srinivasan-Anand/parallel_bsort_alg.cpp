#include<algorithm>
#include "bucket_sort.h"
#include "common_head.h"

using namespace std;

pthread_barrier_t bar1; //initialise barrier

int num_of_buckets;
int maximum;
vector<mutex> mtx;
vector<priority_queue<int, vector<int>, greater<int>>> all_buckets;

void *bucket_sort(void *input) // implementation of the bucketsort algorithm.
{
	struct indices_data *data = (struct indices_data*)input; // local initialisation.
    int start = data->start;
    int end = data->end;
    int tid = data->thread_id;
    pthread_barrier_wait(&bar1);
    if(tid==0)
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
        mtx[bucket_number].lock();
        all_buckets[bucket_number].push(numbers[i]);
        mtx[bucket_number].unlock();
    }
    pthread_barrier_wait(&bar1);
	if(tid==0)
    {
		clock_gettime(CLOCK_MONOTONIC,&end_time);
	}
    return NULL;
}

void parallel_bucket_sort(int num_threads, int chunk_size)
{
    pthread_t* threads = (pthread_t*)malloc((num_threads)*sizeof(pthread_t)); // global initialisation.
    pthread_barrier_init(&bar1, NULL, num_threads);
	size_t current_chunk = chunk_size;

    num_of_buckets = ceil(float(log(numbers.size())));
    maximum = *std::max_element(numbers.begin(), numbers.end());
    std::vector<std::mutex> list(num_of_buckets);
    mtx.swap(list);
    all_buckets.resize(num_of_buckets);

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

	for(int i = 1; i < num_threads; i++) // creating the threads.
    {
        int ret = pthread_create(&(threads[i]), NULL, &bucket_sort, (void *)(&data[i]));
        if(ret)
        {
            printf("ERROR; pthread_create: %d\n", ret);
            exit(-1);
        }
    }

    bucket_sort((void *)(&data[0])); // join with the master thread.

    for(int i=1; i<num_threads; i++) // joining the parallel threads.
    {
        int ret = pthread_join(threads[i],NULL);
        if(ret){
            printf("ERROR; pthread_join: %d\n", ret);
            exit(-1);
        }
    }

    numbers.clear();
    for(int i = 0; i < num_of_buckets; i++){
        while(!all_buckets[i].empty()){
            numbers.push_back(all_buckets[i].top());
            all_buckets[i].pop();
        }
    }
    clock_gettime(CLOCK_MONOTONIC,&end_time);

    free(threads); // free memory
    pthread_barrier_destroy(&bar1);
}