#pragma once

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