#include <stdio.h> // for printf()
#include <stdlib.h> //for calloc() and rand() and atoi()
#include <time.h> // for clock_gettime()
#include <stdint.h> // for uint64_t

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


void calculate_histogram(int * array, uint64_t * hist_array, uint64_t array_size, int hist_interval)
{

	printf("SERIAL IMPLEMENTATION\n");

	uint64_t i;
	for(i = 0; i < array_size; i++)
	{
		hist_array[(array[i] - 1) / hist_interval] += 1;
	}
}


uint64_t timer(int * array, uint64_t * hist_array, int hist_interval, void  (*hist_func)(int *, uint64_t *, uint64_t, int))
{
	uint64_t diff;
	struct timespec start, end;

	clock_gettime(CLOCK_MONOTONIC, &start);

	hist_func(array, hist_array, SIZE, hist_interval);

	clock_gettime(CLOCK_MONOTONIC, &end);

	diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;

	return diff;

}


int main(int argc, char * argv[])
{

	int hist_interval = atoi(argv[1]);

	int * array = array_initializer(SIZE);
	array_loader(array, SIZE, MAX_INT, SEED);

	uint64_t * hist_array = (uint64_t *) calloc(MAX_INT/hist_interval, sizeof(uint64_t));

	long long unsigned int time_diff;

	time_diff = timer(array, hist_array, hist_interval, calculate_histogram);
	
	printf("Interval size = %d\nHist array:\n", hist_interval);

	for(int i = 0; i < MAX_INT/hist_interval; i++)
	{
		printf("%lu\n", hist_array[i]);
	}

	printf("Time taken = %7llu milliseconds\n\n", time_diff / (BILLION/1000));
	
	free(array);

	return 0;

}

