#pragma once
#include <bits/stdc++.h>

using file = std::vector<std::string>;
const long double EPSILON = 1e-8;

struct data {
    int fileType;
    file doc;
};

class NaiveBayes {
public:
    NaiveBayes(int types0, int types1) {
        this->types0 = types0;
        this->types1 = types1;
		fileTypesCount.resize(2);
        totalCount = 0;
    }

    int fileTypePrediction(file& doc) {
        long double prob0 = probability(0, doc);    
        long double prob1 = probability(1, doc);
        // std::cout << "==================\n";  
        // std::cout << "prob0 = " << prob0 << '\n';
        // std::cout << "prob1 = " << prob1 << '\n';
        // std::cout << "==================\n";  

        int prob = (prob0 > prob1 ? 0 : 1); 
        return prob;
    }
    
    void increaseType0() {
        this->types0++;
    }

    void increaseType1() {
        this->types1++;
    }

    void initMaps(const std::vector<data>& dataBayes) {
        for (const auto& data: dataBayes) {
            int fileType = data.fileType;
            file doc = data.doc;
            for (auto& word: doc) {
                wordsInType[fileType][word]++;
                wordsCount[word]++;

                fileTypesCount[fileType]++;
                totalCount++;
            }
        }
    }

private:
    std::vector<int> fileTypesCount; //кол-во слов в каждом типе
    int types0, types1; // количество каждого из типов в обучающих данных
    std::unordered_map<int, std::unordered_map<std::string, int>> wordsInType; // словарь для каждого типа <тип, <слово, кол-во>>
    std::unordered_map<std::string, int> wordsCount; //словарь слов <слово, кол-во>
    int totalCount; // общее количество слов

   long double probability(int fileType, file& test) { // вероятность, что файл test имеет тип fileType
        // std::cout << "TYPE = " << fileType << '\n';
        long double prob1 = 1;
        for (auto word: test) {
            prob1 *= probability_WordInType(word, fileType); 
        }
        long double prob2 = probability_Type(fileType); // P(fileType)
        // std::cout << prob1 << '\n';
        // std::cout << prob2 << '\n';

        return prob1*prob2;
    }

    long double probability_Type(int fileType) {
        return (long double) fileTypesCount[fileType] / totalCount;
    }

    long double probability_WordInType(std::string& word, int fileType) { // P(word|fileType)
        if (!wordsInType[fileType][word]) {
            return EPSILON;
        }
        return (long double) wordsInType[fileType][word] / wordsCount[word]; // кол-во слова в данном типе / общее кол-во слова
    }
};
