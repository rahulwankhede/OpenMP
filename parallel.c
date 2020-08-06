#include <stdio.h> // for printf()
#include <stdlib.h> //for calloc() and rand()
#include <time.h> // for clock_gettime()
#include <stdint.h> // for uint64_t
#include <omp.h> // for #pragma omp parallel

#define SIZE 4294967296 // 4GB
#define MAX_INT 20
#define SEED 10
#define BILLION 1000000000


int * array_initializer(uint64_t size)
//returns an integer array of length size or exits if failed
{
	int * A = (int *) calloc(size, sizeof(int));
	if(A == NULL)
	{
		printf("Could not allocate space in heap");
		exit(1);
	}

	return A;
}


void array_loader(int * array, uint64_t size, int max_int, int seed)
{
	srand(seed);
	for(uint64_t i = 0; i < size; i++)
	{
		array[i] = (rand() % max_int) + 1;
	}
}


void calculate_histogram(int * array, uint64_t * hist_array, uint64_t array_size, int hist_interval, int num_threads)
{
	//int actual_threads;
	omp_set_dynamic(0);
	omp_set_num_threads(num_threads);

	int nBuckets = MAX_INT / hist_interval;

	#pragma omp parallel
	{
		int threads_got = omp_get_num_threads();
		int thread_id = omp_get_thread_num();
		if(thread_id == 0)
		{
			printf("PARALLEL IMPLEMENTATION\t\t\tTHREADS = %d\t\t\tMax Procs = %d\n", threads_got, omp_get_num_procs());
			//actual_threads = threads_got;
			//printf("Threads got = %d\n", threads_got);
		}	

		uint64_t * temp_array = (uint64_t *) calloc(nBuckets, sizeof(uint64_t));
	
		uint64_t i;
		
		for(i = thread_id; i < array_size; i += threads_got)
		{
			temp_array[(array[i] - 1) / hist_interval] += 1;
		}
		
		#pragma omp critical
		{
			for(int i = 0; i < nBuckets; i++)
			{
				hist_array[i] += temp_array[i];
			}
		}

		free(temp_array);
	}
}


uint64_t timer(int * array, uint64_t * hist_array, int hist_interval, int num_threads, void  (*hist_func)(int *, uint64_t *, uint64_t, int, int))
{
	uint64_t diff;
	struct timespec start, end;

	clock_gettime(CLOCK_MONOTONIC, &start);

	hist_func(array, hist_array, SIZE, hist_interval, num_threads);

	clock_gettime(CLOCK_MONOTONIC, &end);

	diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;

	return diff;

}


int main(int argc, char * argv[])
{
	int hist_interval = atoi(argv[1]);
	int num_threads = atoi(argv[2]);

	int * array = array_initializer(SIZE);
	array_loader(array, SIZE, MAX_INT, SEED);

	uint64_t * hist_array = (uint64_t *) calloc(MAX_INT/hist_interval, sizeof(uint64_t));

	long long unsigned int time_diff;

	time_diff = timer(array, hist_array, hist_interval, num_threads, calculate_histogram);

	printf("Interval size = %d\nHist array:\n", hist_interval);

	for(int i = 0; i < MAX_INT/hist_interval; i++)
	{
		printf("%lu\n", hist_array[i]);
	}

	printf("Time taken = %7llu milliseconds\n\n", time_diff / (BILLION/1000));

	free(array);

	return 0;

}




		

