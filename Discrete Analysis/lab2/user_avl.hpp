#pragma once
#include <fstream>
#include <string>
#include <new>
#include <cctype> 

using ull = unsigned long long;

class TUserAvl: public TAvl<std::string, ull> {

    void StrToLow(std::string& str) {
        for (int i = 0; i < str.size(); i++) {
            str[i] = std::tolower(str[i]);
        }
    }

    void Save(std::ostream& os, const TAvlNode* nd) {
		if (nd == nullptr) {
			return;
		}
		int keySize = nd->key.size();
		os.write((char *)&keySize, sizeof(keySize));
		os.write(nd->key.c_str(), keySize);
		os.write((char *)&nd->val, sizeof(nd->val));

        bool left_exist = nd->left != nullptr;
		bool right_exist=  nd->right != nullptr; 
		
		os.write((char *)&left_exist, sizeof(left_exist));
		os.write((char *)&right_exist, sizeof(right_exist));

		if (left_exist) {
			Save(os, nd->left);
		}
		if (right_exist) {
			Save(os, nd->right);
		}
	}

	TAvlNode* Load(std::istream &is) {
		TAvlNode *root = nullptr;
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

		root = new TAvlNode();
		root->key = std::move(key);
		root->val = value;

		if (hasLeft) {
			root->left = Load(is);
		} else {
			root->left = nullptr;
		}
		if (hasRight) {
			root->right = Load(is);
		} else {
			root->right = nullptr;
		}
		return root;
	}

	void OpenFileLoad(std::string &fileName) {
		std::ifstream is{fileName, std::ios::binary | std::ios::in};
//		if (is) {
			DeleteTree(root);
			root = Load(is);
	//	} else {
	//		return;
	//	}
	//	is.close();
	}

	void OpenFileSave(std::string& fileName) {
		std::ofstream os{fileName, std::ios::binary | std::ios::out};
	//	if (os) {
			Save(os, root);
	//	}  else {
	//	    return;
	//	}
	//	os.close();
	}

public:
	void UserInsert() {
		std::string key;
		ull value = 0;
    //    std::cout << "UserInsert: Before cin\n";
		std::cin >> key >> value;

     //   std::cout << "Before str_toLOW\n";
		StrToLow(key);
    //    std::cout << "Before add\n";
		Add(std::move(key), value);
	}

	void UserRemove() {
		std::string key;
		std::cin >> key;
		StrToLow(key);
		DeleteNode(std::move(key));
	}

	void UserFind(const std::string &k) {
		std::string key{k};
		StrToLow(key);
		TAvlNode* res_find = Find(std::move(key));
		if (res_find != nullptr) {
			std::cout << "OK: " << res_find->val << "\n"; 
		}
		else {
			std::cout << "NoSuchWord\n";
		}

	}

	void SaveLoad() {
		std::string comand;
		std::string fileName;
		std::cin >> comand >> fileName;
		if (comand == "Save") {
			OpenFileSave(fileName);
        			
		} else if (comand == "Load") {
			OpenFileLoad(fileName);
		}
		std::cout << "OK\n";
	}
};