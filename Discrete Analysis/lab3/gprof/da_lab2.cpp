#include <iostream>
#include <string>
#include <new>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <cstring>

using ull = unsigned long long;

template<typename K, typename V>
class TAvl {
protected:
	struct TNode {
		K key;
		V value;
		ull height;
		TNode *left;
		TNode *right;
		TNode() : key(), value(), height{1}, left{nullptr}, 
							right{nullptr} {};
		TNode(K k, V v) : key{k}, value{v}, height{1}, 
							left{nullptr}, right{nullptr} {};
	};

	TNode *root;

	ull Height(const TNode *node) {
		return node != nullptr ? node->height : 0;
	}

	int Balance(const TNode *node) {
		return Height(node->left) - Height(node->right);
	}

	void Reheight(TNode *node) {
		node->height = std::max(Height(node->left), Height(node->right)) + 1;
	}

	TNode *RotateLeft(TNode *a) {
		TNode *b = a->right;
		a->right = b->left;
		b->left = a;
		Reheight(a);
		Reheight(b);
		return b;
	}

	TNode *RotateRight(TNode *a) {
		TNode *b = a->left;
		a->left = b->right;
		b->right = a;
		Reheight(a);
		Reheight(b);
		return b;
	}

	TNode *RLrotate(TNode *a) {
		a->right = RotateRight(a->right);
		return RotateLeft(a);
	}

	TNode *LRrotate(TNode *a) {
		a->left = RotateLeft(a->left);
		return RotateRight(a);
	}

	TNode *Rebalance(TNode *node) {
		if (node == nullptr) {
			return nullptr;
		}
		Reheight(node);
		int balanceRes = Balance(node);
		if (balanceRes == -2) {
			if (Balance(node->right) == 1) {
				return RLrotate(node);
			}
			return RotateLeft(node);
		}
		else if (balanceRes == 2) {
			if (Balance(node->left) == -1) {
				return LRrotate(node);
			}
			return RotateRight(node);
		}
		return node;
	}

	TNode *Insert(TNode *node, K k, V v) {
		if (node == nullptr) {
			try {
				node = new TNode(std::move(k), v);
			}
			catch (std::bad_alloc &e) {
				std::cout << "ERROR: " << e.what() << "\n";
				return nullptr;
			}
			std::cout << "OK\n";
			return node;
		}
		if (k < node->key) {
			node->left = Insert(node->left, k, v);
		}
		else if (k > node->key) {
			node->right = Insert(node->right, k, v);
		}
		else {
			std::cout << "Exist\n";
		}
		return Rebalance(node);
	}

	TNode *RemoveMin(TNode *node, TNode *tempNode) {
		if (tempNode->left != nullptr) {
			tempNode->left = RemoveMin(node, tempNode->left);
		}
		else {
			TNode *rightChild = tempNode->right;
			node->key = std::move(tempNode->key);
			node->value = tempNode->value;
			delete tempNode;
			tempNode = rightChild;
		}
		return Rebalance(tempNode);
	}

	TNode *Remove(TNode *node, K k) {
		if (node == nullptr) {
			std::cout << "NoSuchWord\n";
			return nullptr;
		}
		if (k < node->key) {
			node->left = Remove(node->left, k);
		}
		else if (k > node->key) {
			node->right = Remove(node->right, k);
		}
		else {
			TNode *leftChild = node->left;
			TNode *rightChild = node->right;
			if (leftChild == nullptr && rightChild == nullptr) {
				delete node;
				std::cout << "OK\n";
				return nullptr;
			}
			if (rightChild == nullptr) {
				delete node;
				std::cout << "OK\n";
				return leftChild;
			}
			if (leftChild == nullptr) {
				delete node;
				std::cout << "OK\n";
				return rightChild;
			}
			node->right = RemoveMin(node, rightChild);
			std::cout << "OK\n";
		}
		return Rebalance(node);
	}

	TNode *Search(TNode *node, K k) {
		if (node == nullptr) {
			return nullptr;
		}
		for (TNode *iter = node; iter != nullptr; ) {
			if (k < iter->key) {
				iter = iter->left;
			}
			else if (k > iter->key) {
				iter = iter->right;
			}
			else {
				return iter;
			}
		}
		return nullptr;
	} 

	void PrintTree(TNode *node) {
		static int cnt = 0;
		if (node != nullptr) {
			cnt++;
			PrintTree(node->right);
			cnt--;
			for (int i = 0; i < cnt; i++) {
				std::cout << "\t";
			}
			std::cout << node->key << "\n";
			cnt++;
			PrintTree(node->left);
			cnt--;
		}
	}

public:
	TAvl() : root(nullptr) {};

	void TAdd(K k, V v) {
		root = Insert(root, std::move(k), v);
	}

	void TDel(K k) {
		root = Remove(root, std::move(k));
	}

	TNode *TFind(K k) {
		return Search(root, std::move(k));
	}

	void Print() {
		PrintTree(root);
	}

	void TDel_all(TNode *node) {
		if (node != nullptr) {
			TDel_all(node->left);
			TDel_all(node->right);
			delete node;
		}
	}

	~TAvl() {
		TDel_all(root);
	}
};

class TDetailAvl : public TAvl<std::string, ull> {

	void Lower(std::string &str) {
		int strSize = str.size();
		for (int i = 0; i < strSize; i++) {
			str[i] = std::tolower(str[i]);
		}
	}

	void Save(std::ostream &os, const TNode *node) {
		if (node == nullptr) {
			return;
		}
		int keySize = node->key.size();
		os.write((char *)&keySize, sizeof(keySize));
		os.write(node->key.c_str(), keySize);
		os.write((char *)&node->value, sizeof(node->value));

		bool hasLeft = node->left != nullptr;
		bool hasRight = node->right != nullptr;
		
		os.write((char *)&hasLeft, sizeof(hasLeft));
		os.write((char *)&hasRight, sizeof(hasRight));

		if (hasLeft) {
			Save(os, node->left);
		}
		if (hasRight) {
			Save(os, node->right);
		}
	}

	TNode *Load(std::istream &is, const TNode *node) {
		TNode *root = nullptr;

		int keySize;
		is.read((char *)&keySize, sizeof(keySize));

		if (is.gcount() == 0) {
			return root;
		}

		char *key = new char[keySize + 1];
		key[keySize] = '\0';
		is.read(key, keySize);

		ull value;
		is.read((char *)&value, sizeof(value));

		bool hasLeft = false;
		bool hasRight = false;
		is.read((char *)&hasLeft, sizeof(hasLeft));
		is.read((char *)&hasRight, sizeof(hasRight));

		root = new TNode();
		root->key = std::move(key);
		root->value = value;

		if (hasLeft) {
			root->left = Load(is, root);
		}
		else {
			root->left = nullptr;
		}

		if (hasRight) {
			root->right = Load(is, root);
		}
		else {
			root->right = nullptr;
		}

		return root;
	}

	bool OpenFileSave(std::string &fileName) {
		std::ofstream os{fileName.c_str(), std::ios::binary | std::ios::out};
		if (os) {
			Save(os, root);
		} 
		else {
			return false;
		}
		os.close();
		return true;
	}

	bool OpenFileLoad(std::string &fileName) {
		std::ifstream is{fileName.c_str(), std::ios::binary | std::ios::in};
		if (is) {
			TDel_all(root);
			root = Load(is, nullptr);
		}
		else {
			return false;
		}
		is.close();
		return true;
	}

public:
	void DetailInsert() {
		std::string key;
		ull value = 0;

		std::cin >> key >> value;
		Lower(key);

		TAdd(std::move(key), value);
	}

	void DetailRemove() {
		std::string key;

		std::cin >> key;
		Lower(key);

		TDel(std::move(key));
	}

	void DetailFind(const std::string &k) {
		std::string key{k};

		Lower(key);

		TNode *resFind = TFind(std::move(key));
		if (resFind != nullptr) {
			std::cout << "OK: " << resFind->value << "\n"; 
		}
		else {
			std::cout << "NoSuchWord\n";
		}

	}

	void SaveLoad() {
		std::string cmd;
		std::string fileName;

		std::cin >> cmd >> fileName;
		if (cmd == "Save") {
			if (!OpenFileSave(fileName)) {
				std::cout << "ERROR: can't open file\n";
				return;
			}
		}
		else if (cmd == "Load"){
			if (!OpenFileLoad(fileName)) {
				std::cout << "ERROR: can't open file\n";
				return;
			}
		}
		std::cout << "OK\n";
	}


};

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
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