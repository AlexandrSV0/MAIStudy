#include <iostream>
#include <string>
#include "tree.hpp"

int main() {
	std::string textA, textB;
	std::cin >> textA >> textB;

	TSuffixTree tree(textA, textB);
	tree.Search();
	
}