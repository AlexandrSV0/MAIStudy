CPP=g++
CPPFLAGS=-c -Wall -pedantic -std=c++14 -O2
EXECUTABLE=solution

all: $(EXECUTABLE)
	
$(EXECUTABLE): tree.o benchmark.o
	$(CPP) -O2 $^ -o $@ 

benchmark: benchmark.o tree.o
	$(CPP) -Wall -pedantic -std=c++14 -O2 $^ -o $@

# test: test.o 
# 	$(CPP) -O2 $^ -o $@
	
.cpp.o:
	$(CPP) $(CPPFLAGS) $< -o $@ -O2

# Зависимости
main.o : tree.hpp
benchmark.o : tree.hpp

clean:
	rm -f *.o $(EXECUTABLE) benchmark
.PHONY: all clean