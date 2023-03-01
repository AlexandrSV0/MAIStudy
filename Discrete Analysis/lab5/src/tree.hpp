#pragma once
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
