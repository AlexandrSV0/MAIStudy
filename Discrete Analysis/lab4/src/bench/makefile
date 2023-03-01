CC=g++
FLAGS=-std=c++17 -pedantic -Wall -Wextra -Wno-unused-variable
BENCH_OUTPUT=benchmark

all: benchmark.cpp
	$(CC) $(FLAGS) benchmark.cpp -o $(BENCH_OUTPUT)

clean:
	rm *.o $(OUTPUT)