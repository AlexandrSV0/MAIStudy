CC=g++
FLAGS=-std=c++14 -pedantic -Wall -Wextra -Wno-unused-variable
OUTPUT=l1main
OUTPUT_BENCHMARK=benchmark

all: sort.o l1main.cpp
	$(CC) $(FLAGS) sort.o l1main.cpp -o $(OUTPUT)
benchmark: sort.o benchmark.cpp
	$(CC) $(FLAGS) sort.o benchmark.cpp -o $(OUTPUT_BENCHMARK)
sort.o: sort.cpp sort.hpp
	$(CC) $(FLAGS) -c sort.cpp
clean:
	rm *.o $(OUTPUT)