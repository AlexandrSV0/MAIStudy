#pragma once
#include <iostream>
#include <algorithm>
#include <chrono>
#include <vector>

typedef unsigned long long ull;
const int DIGITS = 32;

class TKeyVal {
public:
	char key[DIGITS +1 ];
	ull value;
	TKeyVal& operator=(const TKeyVal& old);
	friend bool operator< (const TKeyVal& st1, const TKeyVal& st2);
};

int GetPos(char c);
void CountingSort(std::vector<TKeyVal>& src, int id);
void RadixSort(std::vector<TKeyVal>& src);