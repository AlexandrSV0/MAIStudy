#include <iostream>
#include <math.h>
#include <vector>
#include <chrono>
using duration = std::chrono::microseconds;


using namespace std;

#define FastIO ios_base::sync_with_stdio(false); cin.tie(nullptr), cout.tie(nullptr); 

void readMatrixAndB(vector<vector<double>>& matrix) {
	int n = matrix.size();
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            std::cin >> matrix[i][j];
    
    for (int i = 0; i < n; i++)
        std::cin >> matrix[i][n];
}

void swapRows(vector<vector<double>>& data, int curRow, int rowToSwap) {
    for (int j = 0; j < data.size()+1; j++) { 
        double cp = data[curRow][j]; 
        data[curRow][j] = data[rowToSwap][j]; 
        data[rowToSwap][j] = cp; 
    }
}

void Gauss(vector<vector<double>>& data, int row) {     
    int column = row;
    for (int i = row + 1; i < data.size(); i++)
        for (int j = column+1; j < data.size()+1; j++)
            data[i][j] += data[row][j] * (-data[i][column] / data[row][column]); 
}

void solveEquatation(vector<vector<double>>& matrix, vector<double>& res) {
    int n = matrix.size();
	for (int i = n-1; i >= 0; i--) { 
        double b_i = matrix[i][n];
        for (int j = n - 1; j > i; j--)
            b_i -= res[j] * matrix[i][j];
        res[i] = b_i / matrix[i][i];
    }
}

int findMaxInColumn(vector<vector<double>>& matrix, int column) {
	double max = matrix[column][column];
	int pos = column;
	for (int i = column + 1; i < matrix.size(); i++) {
		if (matrix[i][column] - max > 0.0001) {
			max = matrix[i][column];
			pos = i;
		}
	}
	return pos;
}

int main() {
    FastIO
    int n;
    cin >> n;
    vector<vector<double>> matrix(n, vector<double>(n+1));
    readMatrixAndB(matrix);
    std::chrono::time_point<std::chrono::high_resolution_clock> start, stop;
    float t = 0;
    start = std::chrono::high_resolution_clock::now();


    for (int j = 0; j < n-1; j++) { 
		int maxPos = findMaxInColumn(matrix, j);
        int i = j; 
        if (maxPos != i) swapRows(matrix, i, maxPos); 

        Gauss(matrix, i); 
    }
	stop = std::chrono::high_resolution_clock::now();
    t += std::chrono::duration_cast<duration>(stop - start).count();
    std::cout << "time = " << t << " ms" << std::endl;

    vector<double> res(n);
    solveEquatation(matrix, res);
  
    // for (int i = 0; i < n; i++)
        // printf("%10e ", res[i]);
}