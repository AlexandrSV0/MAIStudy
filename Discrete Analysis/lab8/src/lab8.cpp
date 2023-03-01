#include <bits/stdc++.h>

struct element {
	double value;
	int line;
	int price;
};


int main() {
    clock_t start = clock();
	int n, m;
	std::cin >> m >> n;
	if (n > m) {//если коэффициентов > чем уравнений, то найти их невозможно
		std::cout << "-1\n";
		return 0;
	}
	std::vector<std::vector<element>> matr(m, std::vector<element> (++n));
	std::vector<int> res;
	// init
	for (int i = 0; i < m; i++) {
		int priceForI;
		for (int j = 0; j < n; j++) {
			double value;
			std::cin >> value;
			matr[i][j] = {value, i+1, -1};
			if (j == n - 1) {
				priceForI = value;
			}
		}
		for (int j = 0; j < n; j++) {
			matr[i][j].price = priceForI;
		}
	}
	// solve
	int curLine=0;
	for (int curRow = 0; curRow < n-1; curRow++) { // столбцы
		int minLine = -1;
		int minPrice = 111;

		for (int i = curLine; i < m; i++) { // проходим по всем строкам одного столбца в поиске ненулевого элемента с минимальной стоимостью
			if (matr[i][curRow].value != 0.0 && matr[i][curRow].price < minPrice) {
				minPrice = matr[i][curRow].price;
				minLine = i;
			}
		}

		if (minLine == -1) {
			std::cout << "-1\n";
			return 0;
			// continue;
		}
	
		res.push_back(matr[minLine][0].line);
		matr[curLine].swap(matr[minLine]); // свапаем самую дешевую строку с ненулевым элементом с самой верхней необработаннной строкой
		for (int i = curLine+1; i < m; i++) { // берем ряды, находящиеся под текущим, и вычитаем из них текущий
			double div = matr[i][curRow].value / matr[curLine][curRow].value;
			for (int k = curRow; k < n; k++) {

				matr[i][k].value -= matr[curLine][k].value * div; 
			}
		}
		curLine++;
	}

	clock_t end = clock();
	std::cout << "Greed time: " << double(end - start) / CLOCKS_PER_SEC << std::endl;
	// sort(res.begin(), res.end());
	// for (int i: res) {
	// 	std::cout << i << " ";
	// }
}

