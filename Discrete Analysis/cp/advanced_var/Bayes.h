#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>

using file = std::vector<std::string>;

struct data {
    file tag;
    file doc;
};

class BayesClassifier {
public:
    void initMaps(std::vector<data>& dataset) {
        for (const auto& data : dataset) {
            file fileTypes = data.tag;
            file doc = data.doc;
            for (auto& type : fileTypes) {
                for (auto& word : doc) {
                    wordsInType[type][word]++;
    				wordsCount[word]++;
    				fileTypesCount[type]++;
                }
            }
            totalCount += doc.size();
        }
    }

    std::unordered_map<std::string, long double> predict(file& doc) {
        std::unordered_map<std::string, long double> probabilities;
        for (auto& data : wordsInType) {
            std::string curType = data.first;
            probabilities[curType] = probability(curType, doc);
        }
        return probabilities;
    }

    void saveStats(std::ofstream &out) {
        out << totalCount << "\n";
        for (auto& dataType : wordsInType) {
            std::string tag = dataType.first;
            auto data = dataType.second;
            out << tag << " " << data.size() << " ";
            for (auto& wordsCount : data) {
                out << wordsCount.first << " " << wordsCount.second << " ";
            }
            out << "\n";
        }
    }

    void loadStats(std::ifstream &in) {
        in >> totalCount;
        std::string tag;
        while (in >> tag) {
            int amountOfWords;
            in >> amountOfWords;
            for (int i = 0; i < amountOfWords; ++i) {
                std::string word;
                int count;
                in >> word >> count;
				wordsInType[tag][word] = count;
				wordsCount[word] += count;
     			fileTypesCount[tag] += count;
            }
        }
    }

    int getTagsCount() {
        return fileTypesCount.size();
    }
 
private:
    long double alpha = 1;
    std::unordered_map<std::string, int> fileTypesCount; //кол-во слов в каждом типе
    std::unordered_map<std::string, std::unordered_map<std::string, int>> wordsInType; // словарь для каждого типа <тип, <слово, кол-во>>
    std::unordered_map<std::string, int> wordsCount; //словарь слов <слово, кол-во>
    int totalCount = 0; // общее количество слов

    long double probability(std::string& fileType, file& doc) { // P(type | doc) вероятность, что файл doc имеет тип fileType
        long double prob = 0;
        for (auto word : doc) {
            prob += log(probability(word, fileType));
        }
        prob += log(probability(fileType));
        return prob;
    }

    long double probability(std::string &tag) { // P(type)
        return (long double) (fileTypesCount[tag] + alpha) / (totalCount + alpha * totalCount);
    }

    long double probability(std::string& word, std::string& fileType) { // P(word | type)
        return  (long double) (wordsInType[fileType][word] + alpha) / (wordsCount[word] + alpha * wordsInType.size());
    }
};