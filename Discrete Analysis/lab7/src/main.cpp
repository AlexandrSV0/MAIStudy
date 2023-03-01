#include <bits/stdc++.h>

#define endl '\n'

enum Operation {
	subtractionOfOne, // -1
	divisionByTwo, // /2 
	divisionByThree // /3
};

void printOperations(const std::vector<Operation> operations, int num) {
	while (num > 1) {
		switch (operations[num]) {
		case subtractionOfOne:
			std::cout << "-1 ";
			--num;
			break;
		case divisionByTwo:
			std::cout << "/2 ";
			num /= 2;
			break;
		case divisionByThree:
			std::cout << "/3 ";
			num  /= 3;
			break;
		default:
			std::cout << "broken" << endl;
			break;
		}
	}
}

int main() {
	// unsigned int start_time =  clock(); // начальное время
    std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);
	long long n; 
	std::cin >> n;
	std::vector<int> prefixSumm(n + 1);
	std::vector<Operation> operations(n + 1, subtractionOfOne);

	for (int i = 2; i < prefixSumm.size(); i++) {
		prefixSumm[i] = prefixSumm[i - 1] + i;

		if (i % 2 == 0 && prefixSumm[i / 2]  + i < prefixSumm[i]) {
			prefixSumm[i] = prefixSumm[i / 2] + i;
			operations[i] = divisionByTwo;
		} 
		if (i % 3 == 0 && prefixSumm[i / 3] + i < prefixSumm[i]) {
			prefixSumm[i] = prefixSumm[i / 3] + i;
			operations[i] = divisionByThree;
		}
		
	}
	float end_time = clock(); // конечное время
    // unsigned int search_time = end_time - start_time;
    std::cout << "Search time DP = " << end_time / CLOCKS_PER_SEC  << '\n';
	// std::cout << prefixSumm[n] << endl;

	// printOperations(operations, n);
}