#include <iostream>
#include <fstream>
#include <vector>
#include <random>

using namespace std;
#define FastIO ios_base::sync_with_stdio(false); cin.tie(nullptr), cout.tie(nullptr); 

int main() {
	FastIO
    vector<int> tests ={2, 10, 100, 1000, 10000};
    srand(time(NULL));
    for (auto t : tests) {
        string s = to_string(t);
        string filen = "test" + s + ".txt";

        ofstream file;
		file.open(filen,  ios::app);
        if (file.is_open()) {
			int n = t;
			file << n << "\n";

            for (int i = 0; i < n+1; i++) { // +1 for B vector
				for (int j = 0; j < n; j++) {
					int x = rand();
					file << x << ' ';
				}
				file << '\n';
            }
            file.close();
        } else{
            return 1;
        }

    }
    return 0;
}