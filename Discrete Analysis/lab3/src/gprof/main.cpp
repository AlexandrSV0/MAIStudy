#include "detail_avl.h"

int main() {
	//std::ios::sync_with_stdio(false);
	//std::cin.tie(nullptr);
	std::string cmd;
	TDetailAvl tree;

	while (std::cin >> cmd) {
		if (cmd[0] == '+') {
			tree.DetailInsert();
		}
		else if(cmd[0] == '-') {
			tree.DetailRemove();
		}
		else if(cmd[0] == '!' && cmd.size() == 1) {
			tree.SaveLoad();
		}
		/*else if (std::strcmp(cmd.c_str(), "Print") == 0) {
			tree.Print();
		}*/
		else {
			tree.DetailFind(std::move(cmd));
		}
	}

	return 0;
}