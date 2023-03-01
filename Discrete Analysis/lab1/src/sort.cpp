#include "sort.hpp"

TKeyVal& TKeyVal::operator= (const TKeyVal& old) {
	for (int i = 0; i < 32; i++)
		key[i] = old.key[i];
	value = old.value;
	return *this;
}

bool operator< (const TKeyVal& st1, const TKeyVal& st2) {
	return (st1.key < st2.key ? true : false);
}

int GetPos(char c) {
	if (c >= '0' && c <= '9') {
		return c - '0';
	}
	else if (c >= 'a' && c <= 'f') {
 		return c - 'a' + 10;
	}
	else if (c >= 'A' && c <= 'F') {
		return c - 'A' + 10;
	}
    else {
		return 0;
	}
}

void CountingSort(std::vector<TKeyVal>& src, int id) {
	std::vector<TKeyVal> output(src.size());
	ull count[16] = { 0 };

	for (ull i = 0; i < src.size(); i++) {
		count[GetPos(src[i].key[id])]++;
	}

	for (int i = 1; i < 16; i++) {
		count[i] += count[i - 1];
	}

	for (int i = src.size() - 1; i >= 0; i--) {
		output[count[GetPos(src[i].key[id])] - 1] = src[i];
		count[GetPos(src[i].key[id])]--;
	}

	src = output;
}

void RadixSort(std::vector<TKeyVal>& src) {
	int DIGITS = 32;
	for (int id = DIGITS - 1; id >= 0; id--) {
		CountingSort(src, id);
	}
}