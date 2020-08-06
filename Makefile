CC=gcc

all: serial parallel

serial: serial.o
	$(CC) -fopenmp -o serial serial.o

serial.o: serial.c
	$(CC) -fopenmp -o serial.o -c serial.c


parallel: parallel.o
	$(CC) -fopenmp -o parallel parallel.o

parallel.o: parallel.c
	$(CC) -fopenmp -o parallel.o -c parallel.c


clean:
	rm -f serial.o serial parallel.o parallel job*

