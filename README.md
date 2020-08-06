# OpenMP

## Histogram calculation using OpenMP

Given a large array of integers (in our case, 16 GB = 2 to the 32 ints) and the range to divide the array into, this code creates a histogram over the array into the range given. We use OpenMP to divide the work among multiple processes to achieve a considerable speedup over serial implementation.
