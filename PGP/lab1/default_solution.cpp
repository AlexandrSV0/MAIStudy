#include<iostream>
#include <chrono>
#include <vector>

using namespace std;

vector<int> find_maximums(vector<int> v1, vector<int> v2, vector<int> res, int n) {
	for (int i = 0; i < n; i++) {
		res[i] = v1[i] > v2[i] ? v1[i] : v2[i];
	}
	return res;
}

int main() {
	int n; 
	cin >> n;
	vector<int> v1(n), v2(n);
	for (int i = 0; i < n; i++) {
		cin >> v1[i];
	}
	for (int i = 0; i < n; i++) {
		cin >> v2[i];
	}
	vector<int> res(n);
	auto start = chrono::high_resolution_clock::now();
	res = find_maximums(v1, v2, res, n);
	auto end = chrono::high_resolution_clock::now();
	double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    time_taken *= 1e-9;
    printf("Time taken by program is : %.10lf sec", time_taken);

	// for (int i = 0; i < n; i++) {
	// 	cout << res[i] << " ";
	// }
} 