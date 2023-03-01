#include <cstring>
#include <fstream>
#include <unordered_set>
#include <cmath>
#include "Bayes.h"
using namespace std;

file makeVector(std::string &text) { //преобразование текста каждого файла в вектор слов
    std::vector<std::string> words;
    std::string readingWord;

	for (int i = 0; i < text.size(); i++) {
		char c = tolower(text[i]);
		if (c >= 'a' && c <= 'z') {
			readingWord += c;
		} else if (readingWord.length() > 0) {
			words.push_back(readingWord);
			readingWord = "";
		}
		if (i + 1 == text.size() && readingWord.length() > 0) {
			words.push_back(readingWord);
		}
	}
	return words;
}	

std::unordered_map<std::string,long double> softmax(std::unordered_map<std::string, long double>& probs);

void printError() {
    std::cerr << "Invalid syntax\n" 
              "./prog learn --input <input file> --output <stats file>\n"
              "./prog classify --stats <stats file> --input <input file> --output <output file>\n";
}

void printEmptyError() {
    std::cerr << "Invalid syntax:\n"
                "<input file> (or <output file>) field is empty\n";
}

bool checkParamLearn(int argc, char* argv[]) {
    if (argc == 6 && !strcmp(argv[2], "--input")  && !strcmp(argv[4], "--output") && strcmp(argv[3], argv[5])) {
        return true;
    } else  {
        printError();
        return false;
    }
}

bool checkParamClassify(int argc, char* argv[]) {
    if (argc == 8 && !strcmp(argv[2], "--stats") && !strcmp(argv[4], "--input") && !strcmp(argv[6], "--output") && strcmp(argv[3], argv[5]) && strcmp(argv[5], argv[7]) && strcmp(argv[3], argv[7])) {
        return true;
    } else {
        printError();
        return false;
    }
}

void readText(std::ifstream& inStream, ::string& text, int lines) {
    for (int i = 0; i < lines + 1; ++i) {
        std::string cur_line;
        getline(inStream, cur_line);
        text += " " + cur_line;
    }
}

void readFileToDataset(std::ifstream& inStream, std::vector<data>& dataset) {
    int lines;
    while (inStream >> lines) {
        std::string types;
        std::string text;
        inStream.ignore();
        getline(inStream, types);
        readText(inStream, text, lines);       
        dataset.push_back({makeVector(types), makeVector(text)});
    }
}

void writeToOutStream(std::ofstream& outStream, std::unordered_map<std::string, long double> preds, double threshold) {
    bool comma = false;
    for (const auto& pred : preds) {
        if (pred.second > threshold) {
            if (comma) {
                outStream << ", ";
            }
            outStream << pred.first;
            comma = true;
        }
    }
    outStream << "\n";
}

bool wrongArgsCount(int argc) {
    return argc < 6;
}


std::unordered_map<std::string,long double> softmax(std::unordered_map<std::string, long double>& probs) {
    long double max = -1e10;
    for (const auto& prob : probs) {
        if (prob.second > max) {
            max = prob.second;
        }
    }

    long double sum = 0;
    for (auto& prob : probs) {
        sum += exp(prob.second - max);
    }

    long double constant = max + log(sum);
    std::unordered_map<std::string, long double> res;
    for (auto prob : probs) {
        res[prob.first] = exp(prob.second - constant);
    }
    return res;
}

void makePredictAndWriteOutput(std::ifstream& inStream, std::ofstream& outStream, BayesClassifier& BC) {
    int lines;
    while (inStream >> lines) {
        std::string text;
        inStream.ignore();
        readText(inStream, text, lines);
        file doc = makeVector(text);
        
        std::unordered_map<std::string, long double> probas = BC.predict(doc);
        std::unordered_map<std::string,long double> preds = softmax(probas);
        
        double threshold = 1. / BC.getTagsCount();
        writeToOutStream(outStream, preds, threshold);
    }
}

int main(int argc, char* argv[]) {
    std::string inFile;
    std::string statsFile;
    std::string outFile;
    if (wrongArgsCount(argc)) {
        printError();
        return 1;
    }

    if (!strcmp(argv[1], "learn")) {
        if (!checkParamLearn(argc, argv)) {
            return 1;
        }
        inFile = argv[3];
        statsFile = argv[5];

        std::ifstream inStream(inFile);
        std::ofstream statsStream(statsFile);
        std::vector<data> dataset;
        readFileToDataset(inStream, dataset);
        BayesClassifier BC;
        BC.initMaps(dataset);
        BC.saveStats(statsStream);

    } else if (argc >= 6 && !strcmp(argv[1], "classify")) {
        if (!checkParamClassify(argc, argv)) {
            return 1;
        }
        statsFile = argv[3];
        inFile = argv[5];
        outFile = argv[7];

        std::ifstream inStream(inFile);
        std::ifstream statsStream(statsFile);
        std::ofstream outStream(outFile);

        BayesClassifier BC;
        BC.loadStats(statsStream);
        makePredictAndWriteOutput(inStream, outStream, BC);
    } else {
        printError();
        return 1;
    }
}

