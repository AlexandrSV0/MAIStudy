#include <bits/stdc++.h>
#define endl '\n'


void initGraph(std::vector<std::vector<int>>& graph, int m) {
	while (m--) {
		int i, j;
		std::cin >> i >> j;
		i--; j--;
		graph[i].push_back(j);
		graph[j].push_back(i);
	}
}

void printGraph(const std::vector<std::vector<int>>& graph) {
	int n = graph.size();
	std::cout << n << endl;
	for (int i = 0; i < n; i++) {
		std::cout << i+1 << ": ";
		for (int j = 0; j < graph[i].size(); j++) {
			std::cout << graph[i][j] +1 << ' ';
		}
		std::cout << endl;
	}
}

void dfs(const std::vector<std::vector<int>>& graph, std::vector<bool>& used, std::vector<std::vector<int>>& res, int v) {
	used[v] = true;
	res.back().push_back(v);
	for (int i = 0; i < graph[v].size(); i++) {
		int u = graph[v][i];
		if (!used[u]) {
			dfs(graph, used, res, u);
		}
	}
}

void solve(const std::vector<std::vector<int>>& graph, int n) {
	std::vector<std::vector<int>> res;
	std::vector<bool> used(n);
	for (int i = 0; i < n; i++) {
		if (!used[i]) {
			// res.clear();
			res.push_back(std::vector<int>());
			dfs(graph, used, res, i);
		}
	}
	for (std::vector<int>& comp: res) {
		std::sort(comp.begin(), comp.end());
		for (int k = 0; k < comp.size(); k++) {
			std::cout << comp[k] + 1 << " ";
		}
		std::cout << '\n';
	}
}

int main() {
	int n, m; 
	std::cin >> n >> m;
	std::vector<std::vector<int>> graph(n);
	initGraph(graph, m);
	solve(graph, n);
	float end = clock();
	std::cout << "Time: " << end / CLOCKS_PER_SEC << endl;
	// printGraph(graph);
	return 0;
}