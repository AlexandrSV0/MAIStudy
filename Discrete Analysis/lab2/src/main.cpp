#include <iostream>
#include <cstring>
#include "avl.hpp"
#include "user_avl.hpp"

using ull = unsigned long long;

int main() {
	std::ios::sync_with_stdio(false);
	std::string comand;
	TUserAvl tr;
	while (std::cin >> comand) {
		if (comand[0] == '+') {
      //      std::cout << "main: if: in +\n";
			tr.UserInsert();
		}
		else if (comand[0] == '-') {
			tr.UserRemove();
		}
		else if (comand[0] == '!' && comand.size() == 1) {
			tr.SaveLoad();
		}
		else {
			tr.UserFind(std::move(comand));
		}
	}
	return 0;
}
