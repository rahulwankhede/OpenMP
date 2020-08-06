#include <stdio.h> // for printf()
#include <stdlib.h> //for calloc() and rand()
#include <time.h> // for clock_gettime()
#include <stdint.h> // for uint64_t

#define SIZE 20000
#define MAX_INT 20
#define SEED 10
#define HIST_INTERVAL 4 //1, 2, 4, 5, 10
#define BILLION 1000000000

int * array_initializer(int size)
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


void array_loader(int * array, int size, int max_int, int seed)
{
	srand(seed);
	for(int i = 0; i < size; i++)
	{
		array[i] = (rand() % max_int) + 1;
	}
}


void calculate_histogram(int * array, int * hist_array, int array_size, int hist_interval)
{
	int i;
	for(i = 0; i < array_size; i++)
	{
		hist_array[(array[i] - 1) / hist_interval] += 1;
	}
}


uint64_t timer(int * array, int * hist_array, void  (*hist_func)(int *, int *, int, int))
{
	uint64_t diff;
	struct timespec start, end;

	clock_gettime(CLOCK_MONOTONIC, &start);

	hist_func(array, hist_array, SIZE, HIST_INTERVAL);

	clock_gettime(CLOCK_MONOTONIC, &end);

	diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;

	return diff;

}


int main()
{
	int * array = array_initializer(SIZE);
	array_loader(array, SIZE, MAX_INT, SEED);

	int * hist_array = calloc(MAX_INT/HIST_INTERVAL, sizeof(int));

	long long unsigned int time_diff;

	time_diff = timer(array, hist_array, calculate_histogram);

	for(int i = 0; i < MAX_INT/HIST_INTERVAL; i++)
	{
		printf("%d\n", hist_array[i]);
	}

	printf("\nTime taken = %1.9llu nanoseconds\n", time_diff);

	return 0;

}




		

