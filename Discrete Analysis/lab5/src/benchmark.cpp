#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include "tree.hpp"

using TDuration = std::chrono::microseconds;
const std::string DURATION_PREFIX = "us";

void DefaultSolution(const std::string& textA, const std::string& textB) {
    for(int i = 0; i < textA.size(); ++i) {
        for(int j = i + 1; j < textA.size(); ++j) {
            textB.find(textA.substr(i, j - i));
        }
    }
}

void MySolution(const std::string& textA, const std::string& textB) {
    TSuffixTree tree(textA, textB);
    tree.Search();
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::string textA, textB;
    std::cin >> textA >> textB;

    // Тест моего алгоритма
    {
        std::chrono::time_point<std::chrono::system_clock> startTs = std::chrono::system_clock::now();
        
        MySolution(textA, textB);
        
        auto endTs = std::chrono::system_clock::now();
        uint64_t diffTs = std::chrono::duration_cast<TDuration>( endTs - startTs ).count();
        std::cout << "My Solution : " << diffTs << DURATION_PREFIX << '\n';
    }

    // Тест Наивного алгоритма
    {
        std::chrono::time_point<std::chrono::system_clock> startTs = std::chrono::system_clock::now();
        
        DefaultSolution(textA, textB);
        
        auto endTs = std::chrono::system_clock::now();
        uint64_t diffTs = std::chrono::duration_cast<TDuration>( endTs - startTs ).count();
        std::cout << "Default Solution : " << diffTs << DURATION_PREFIX << '\n';
    }

    return 0;
}