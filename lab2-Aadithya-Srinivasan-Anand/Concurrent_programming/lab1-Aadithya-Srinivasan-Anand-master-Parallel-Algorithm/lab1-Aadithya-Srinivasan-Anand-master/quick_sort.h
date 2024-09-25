#include <stdlib.h>

void parallel_quick_sort(int num_threads, int chunk_size);
void *quick_sort(void *data);
int breakdown(int start, int end);
void *combine_two(void *input);
void combine_k(size_t chunk_size, int num_threads);