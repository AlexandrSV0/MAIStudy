 #include <iostream>
#include <fstream>
#include <string>
#include <new>
#include <cctype> 
#include <algorithm>
#include <cstring>

using ull = unsigned long long;

template<typename typeK, typename typeV>
class TAvl {
protected:

    struct TAvlNode {
        typeK key;
        typeV val;
        ull height;
        TAvlNode* left;
        TAvlNode* right;

        TAvlNode(): key(), val(), height(1), left(nullptr), right(nullptr) {};
        TAvlNode(typeK k, typeV v): key(k), val(v), height(1), left(nullptr), right(nullptr) {};
     
    };
    TAvlNode* root;


    ull GetHeight(const TAvlNode*  nd) {
        return nd == nullptr ? 0 : nd->height; 
    }

    ull GetBalance(const TAvlNode* nd) {
        return GetHeight(nd->left) - GetHeight(nd->right);
    }

    void CalcHeight(TAvlNode* nd) {
        nd->height = std::max(GetHeight(nd->left), GetHeight(nd->right)) + 1;
    }

    TAvlNode* Rotate_LL(TAvlNode* nd) {
        TAvlNode* nd_new = nd->right;
        nd->right = nd_new->left;
        nd_new->left  = nd;
        CalcHeight(nd);
        CalcHeight(nd_new);
        return nd_new;
    }

    TAvlNode* Rotate_RR(TAvlNode* nd) {
        TAvlNode* nd_new = nd->left;
        nd->left = nd_new->right;
        nd_new->right = nd;
        CalcHeight(nd);
        CalcHeight(nd_new);
        return nd_new;
    }

    TAvlNode* Rotate_RL(TAvlNode* nd) {
        nd->right = Rotate_RR(nd->right);
        return Rotate_LL(nd);
    }

    TAvlNode* Rotate_LR(TAvlNode* nd) {
        nd->left = Rotate_LL(nd->left);
        return Rotate_RR(nd);
    }

    TAvlNode* DoBalance(TAvlNode* nd) {
        if (nd == nullptr) {
            return nullptr;
        }
        CalcHeight(nd);
        int nd_balance = GetBalance(nd);
        if (nd_balance == -2) {
            if (GetBalance(nd->right) == 1) {
                return Rotate_RL(nd);
            }
            return Rotate_LL(nd);
        } else if (nd_balance == 2) {
            if (GetBalance(nd->left) == -1) {
                return Rotate_LR(nd);
            }
            return Rotate_RR(nd);
        }
        return nd;
    }

    TAvlNode* Insert(TAvlNode* nd, typeK k, typeV v) {
        if (nd == nullptr) {
            try {
                nd = new TAvlNode(std::move(k), v);
            } catch (std::bad_alloc  &err) {
                std::cout << "ERROR: " << err.what() << "\n";
                return nullptr;
            }
            std::cout << "OK\n";
            return nd;
        }
        if (k < nd->key) {
            nd->left = Insert(nd->left, k, v);
        } else if (k > nd->key) {
            nd->right = Insert(nd->right, k, v);
        } else {
            std::cout << "Exist\n";
        }
//        std::cout << "end insert\n";
        return DoBalance(nd);
    }

 	TAvlNode* RemoveMinRight(TAvlNode* nd, TAvlNode* tmp_nd) {
		if (tmp_nd->left != nullptr) {
			tmp_nd->left = RemoveMinRight(nd, tmp_nd->left);
		}
		else {
			TAvlNode* right_ch = tmp_nd->right;
			nd->key = std::move(tmp_nd->key);
			nd->val = tmp_nd->val;
			delete tmp_nd;
			tmp_nd = right_ch;
		}
		return DoBalance(tmp_nd);
	}

    TAvlNode* RemoveNode(TAvlNode* nd, typeK k) {
        if (nd == nullptr) {
            std::cout << "NoSuchWord\n";
            return nullptr;
        }
        if (k < nd->key) {
            nd->left = RemoveNode(nd->left, k);
        } else if (k > nd->key) {
            nd->right = RemoveNode(nd->right, k);
        } else {
            TAvlNode* nd_left = nd->left;
            TAvlNode* nd_right = nd->right;
            if (nd_left == nullptr && nd_right == nullptr) {
                delete nd;
                std::cout << "OK\n";
                return nullptr;
            }
            if (nd_left == nullptr) {
                delete nd;
                std::cout << "OK\n";
                return nd_right;
            }
            if (nd_right == nullptr) {
                delete nd;
                std::cout << "OK\n";
                return nd_left;
            }
            nd->right = RemoveMinRight(nd, nd_right);
            std::cout << "OK\n";
        }
        return DoBalance(nd);
    }

    TAvlNode* SearchNode(TAvlNode* nd, typeK k) {
		if (nd == nullptr) {
			return nullptr;
		}
		if (k < nd->key) {
            return SearchNode(nd->left, k);
        } else if (k > nd->key) {
            return SearchNode(nd->right, k);
        } else {
            return nd;
        }
    }

    void PrintTree(TAvlNode* nd) {
        static int count = 0;
        if (nd != nullptr) {
            count++;
            PrintTree(nd->right);
            count--;
            for (int i = 0; i < count; i++) {
                std::cout << "\t";
            }
            std::cout << nd->key << "\n";
            count++;
            PrintTree(nd->left);
            count--;
        }
    }


public:
    TAvl(): root(nullptr) {};

    void Add(typeK k, typeV v) {
        root = Insert(root, std::move(k), v);
    }
    
    void DeleteNode(typeK k) {
        root = RemoveNode(root, std::move(k));
    }

    TAvlNode* Find(typeK k) {
        return SearchNode(root, std::move(k));
    }

    void Print() {
        PrintTree(root);
    }

    void DeleteTree(TAvlNode* nd) {
        if (nd != nullptr) {
			DeleteTree(nd->left);
			DeleteTree(nd->right);
			delete nd;
		}
    }

    ~TAvl() {
        DeleteTree(root);
    }
};

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
