#include <bits/stdc++.h>

const char FIRST_SEP = '#';
const char SECOND_SEP = '$';


struct TNode {
	std::map<char, TNode*> Next;
	TNode* suffixLink;

	int begin, end;

	TNode(int begin, int end);
	~TNode();
};

class TSuffixTree {
public:
	std::string text;
	TNode* root;
	TNode* curNode;
	int end;
	int remainder;
	int curLen;
	int curEdge;

	void Search();
	TSuffixTree(const std::string& textA, const std::string& textB);
	~TSuffixTree();
private:
	int indFirst_Sep;
	int DFS(std::vector<std::pair<int,int>>& ans, int& maxLen, TNode* node, int len, int begin);
	void AddLetter(int i);
	void printVector(const std::vector<std::pair<int, int>>& ans);
};

TNode::TNode(int begin, int end): begin(begin), end(end), suffixLink(nullptr) {
}

TNode::~TNode() {
	for(auto& it : Next) {
		delete it.second;
	}
}

TSuffixTree::TSuffixTree(const std::string& textA, const std::string& textB) {
	root = new TNode(0, 0);
	root->suffixLink = root;

	curNode = root;
	curLen = 0;
	curEdge = 0;
	remainder = 0;
	indFirst_Sep = textA.size();

	text = textA + FIRST_SEP + textB + SECOND_SEP;

	end = 0;
	for(int i = 0; i < (int)text.length(); i++){
		AddLetter(i);
		end++;
	}
}

void TSuffixTree::AddLetter(int i){
	TNode* prevAdded = root;
	++remainder;

	if (remainder == 1) {
		curEdge = i;
	}

	while (remainder > 0) { // проверить, есть ли суфф ссылки на корень
		auto it = curNode->Next.find(text[curEdge]);
		TNode* nextNode = nullptr;
		if (it != curNode->Next.end()) {
			nextNode = it->second;
		}

		if (nextNode == nullptr) { // нет пути по букве в позиции curEdge
			curNode->Next[text[curEdge]] = new TNode(i, -1);
			if (prevAdded != root) { // создаем суфф ссылку из предыдущей добавленной вершины в текущую
				prevAdded->suffixLink = curNode;
			}
			prevAdded = curNode;
		} else { // путь по curEdge есть
			int edgeLen;
			if (nextNode->end == -1) { // если лист
				edgeLen = end - nextNode->begin + 1;
			} else { // не лист
				edgeLen = nextNode->end - nextNode->begin;
			}	

			if (curLen >= edgeLen) { // можно ли пройти ребро целиком
				// std::cout << "curLen >= edgeLen\n";
				// std::cout << "I = " << i << '\n';
				curNode = nextNode;
				curLen -= edgeLen;
				curEdge += edgeLen;
				continue;  // проходим ребро целиком
			}

			if (text[nextNode->begin + curLen] == text[i]) { // добавляемая буква есть на ребре после проверенного суффикса (curLen)
				curLen++;
				if (prevAdded != root) { 
	//				std::cout << "suff2\n";
					prevAdded->suffixLink = curNode;
				}
				break; 
			}

			TNode* midNode = new TNode(nextNode->begin, nextNode->begin + curLen);
			curNode->Next[text[curEdge]] = midNode;
			midNode->Next[text[i]] = new TNode(i, -1);
			nextNode->begin += curLen;
			midNode->Next[text[nextNode->begin]] = nextNode;

			if (prevAdded != root) {
//				std::cout << "suff3\n";
				prevAdded->suffixLink = midNode;
			}
			prevAdded = midNode;
		}

		
		if (curNode == root && curLen > 0) {
			curEdge++;
			curLen--;
		} else if (curNode != root) {
			curNode = curNode->suffixLink;
		}
				
		--remainder;
	}
}

int TSuffixTree::DFS(std::vector<std::pair<int,int>>& ans, int& maxLen, TNode* node, int len, int begin) {
	if (node->end == -1 && node->begin <= indFirst_Sep) {
		return 1;
	}
	if (node->end == -1 && node->begin > indFirst_Sep) {
		return 2; 
	}

	bool firstChecked = false;
	bool secondChecked = false;
	
	len = len + node->end - node->begin;
	begin = node->end - len;

	for (auto& it : node->Next) {
		int res = DFS(ans, maxLen, it.second, len, (len == 0 ? it.second->begin : begin));
		if (res == 1) {
			firstChecked = true;
		}
		if (res == 2) {
			secondChecked = true;
		}
	}

	if (len != 0 && firstChecked && secondChecked) {
		if(maxLen == len) {
			ans.push_back({begin, len});
		} else if (maxLen < len) {
			maxLen = len;
			ans.clear();
			ans.push_back({begin, len});
		}     
	}

	if (firstChecked && !secondChecked) {
		return 1;
	}
	if (secondChecked && !firstChecked) {
		return 2;
	}
	return 0;
}

void TSuffixTree::printVector(const std::vector<std::pair<int, int>>& vec) {
	for (auto& element: vec) {
		std::cout  << text.substr(element.first, element.second) << '\n';
	}
}

void TSuffixTree::Search() {
	std::vector<std::pair<int,int>> ans;
	int maxLen = 0;
	DFS(ans, maxLen, root, 0, 0);

	std::cout << maxLen << '\n';
	printVector(ans);
}

TSuffixTree::~TSuffixTree() {
	delete root;
}


int main() {
	std::string textA, textB;
	std::cin >> textA >> textB;

	TSuffixTree tree(textA, textB);
	tree.Search();
	
}