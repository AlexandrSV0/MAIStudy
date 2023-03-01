CC=g++
FLAGS=-std=c++17 -pedantic -Wall -Wextra -Wno-unused-variable
OUTPUT=da_lab2

all: da_lab2.cpp
	$(CC) $(FLAGS) da_lab2.cpp -o $(OUTPUT)

bencmark: benchmark.cpp
	$(CC) $(FLAGS) benchmark.cpp -o benchmark

clean:
	rm *.o $(OUTPUT)