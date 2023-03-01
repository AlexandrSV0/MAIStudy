#ifndef AVL_H
#define AVL_H 1

#include <iostream>
#include <string>
#include <new>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <cstring>


using TUll = unsigned long long;

template<typename K, typename V>
class TAvl {
protected:
	struct TAvlNode {
		K key;
		V value;
		unsigned long long height;
		TAvlNode *left;
		TAvlNode *right;
		TAvlNode() : key(), value(), height{1}, left{nullptr}, 
							right{nullptr} {};
		TAvlNode(K k, V v) : key{k}, value{v}, height{1}, 
							left{nullptr}, right{nullptr} {};
	};

	TAvlNode *root;

	unsigned long long Height(const TAvlNode *node) {
		return node != nullptr ? node->height : 0;
	}

	int Balance(const TAvlNode *node) {
		return Height(node->left) - Height(node->right);
	}

	void Reheight(TAvlNode *node) {
		node->height = std::max(Height(node->left), Height(node->right)) + 1;
	}

	TAvlNode *RotateLeft(TAvlNode *a) {
		TAvlNode *b = a->right;
		a->right = b->left;
		b->left = a;
		Reheight(a);
		Reheight(b);
		return b;
	}

	TAvlNode *RotateRight(TAvlNode *a) {
		TAvlNode *b = a->left;
		a->left = b->right;
		b->right = a;
		Reheight(a);
		Reheight(b);
		return b;
	}

	TAvlNode *BigRotateLeft(TAvlNode *a) {
		a->right = RotateRight(a->right);
		return RotateLeft(a);
	}

	TAvlNode *BigRotateRight(TAvlNode *a) {
		a->left = RotateLeft(a->left);
		return RotateRight(a);
	}

	TAvlNode *Rebalance(TAvlNode *node) {
		if (node == nullptr) {
			return nullptr;
		}
		Reheight(node);
		int balanceRes = Balance(node);
		if (balanceRes == -2) {
			if (Balance(node->right) == 1) {
				return BigRotateLeft(node);
			}
			return RotateLeft(node);
		}
		else if (balanceRes == 2) {
			if (Balance(node->left) == -1) {
				return BigRotateRight(node);
			}
			return RotateRight(node);
		}
		return node;
	}

	TAvlNode *InsertPrint(TAvlNode *node, K k, V v) {
		if (node == nullptr) {
			try {
				node = new TAvlNode(std::move(k), v);
			}
			catch (std::bad_alloc &e) {
				std::cout << "ERROR: " << e.what() << "\n";
				return nullptr;
			}
			std::cout << "OK\n";
			return node;
		}
		if (k < node->key) {
			node->left = InsertPrint(node->left, k, v);
		}
		else if (k > node->key) {
			node->right = InsertPrint(node->right, k, v);
		}
		else {
			std::cout << "Exist\n";
		}
		return Rebalance(node);
	}

	TAvlNode *Insert(TAvlNode *node, K k, V v) {
		if (node == nullptr) {
			try {
				node = new TAvlNode(k, v);//попробуй потом std::move(key)
			}
			catch (std::bad_alloc &e) {
				return nullptr;
			}
			return node;
		}
		if (k < node->key) {
			node->left = Insert(node->left, k, v);
		}
		else if (k > node->key) {
			node->right = Insert(node->right, k, v);
		}
		return Rebalance(node);
	}

	TAvlNode *RemoveMin(TAvlNode *node, TAvlNode *tempNode) {
		if (tempNode->left != nullptr) {
			tempNode->left = RemoveMin(node, tempNode->left);
		}
		else {
			TAvlNode *rightChild = tempNode->right;
			node->key = std::move(tempNode->key);
			node->value = tempNode->value;
			delete tempNode;
			tempNode = rightChild;
		}
		return Rebalance(tempNode);
	}

	TAvlNode *Remove(TAvlNode *node, K k) {
		if (node == nullptr) {
			return nullptr;
		}
		if (k < node->key) {
			node->left = Remove(node->left, k);
		}
		else if (k > node->key) {
			node->right = Remove(node->right, k);
		}
		else {
			TAvlNode *leftChild = node->left;
			TAvlNode *rightChild = node->right;
			if (leftChild == nullptr && rightChild == nullptr) {
				delete node;
				return nullptr;
			}
			if (rightChild == nullptr) {
				delete node;
				return leftChild;
			}
			if (leftChild == nullptr) {
				delete node;
				return rightChild;
			}
			node->right = RemoveMin(node, rightChild);
		}
		return Rebalance(node);
	}

	TAvlNode *RemovePrint(TAvlNode *node, K k) {
		if (node == nullptr) {
			std::cout << "NoSuchWord\n";
			return nullptr;
		}
		if (k < node->key) {
			node->left = RemovePrint(node->left, k);
		}
		else if (k > node->key) {
			node->right = RemovePrint(node->right, k);
		}
		else {
			TAvlNode *leftChild = node->left;
			TAvlNode *rightChild = node->right;
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

	TAvlNode *Search(TAvlNode *node, K k) {
		if (node == nullptr) {
			return nullptr;
		}
		for (TAvlNode *iter = node; iter != nullptr; ) {
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

	void PrintTree(TAvlNode *node) {
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

	void Add(K k, V v) {
		root = Insert(root, std::move(k), v);
	}

	void AddPrint(K k, V v) {
		root = InsertPrint(root, std::move(k), v);
	}

	void Delete(K k) {
		root = Remove(root, std::move(k));
	}

	void DeletePrint(K k) {
		root = RemovePrint(root, std::move(k));
	}

	TAvlNode *Find(K k) {
		return Search(root, std::move(k));
	}

	void Print() {
		PrintTree(root);
	}

	void TreeDelete(TAvlNode *node) {
		if (node != nullptr) {
			TreeDelete(node->left);
			TreeDelete(node->right);
			delete node;
		}
	}

	~TAvl() {
		TreeDelete(root);
	}
};

#endif //AVL_H