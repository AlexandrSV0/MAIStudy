#include "NaiveBayes.h"

/* Наивный байесовский алгоритм, вероятность:
/                        P(Y | X) * P(X)   
/       P(X|Y). = ----------------- , где X - тип файла, Y - данные файла (набор слов)
/                              P(Y)               
/       P(Y | X) * P(X)- вер-ть, что каждое из слов в Y встретится в типе файла X * (вер-ть встретить тип файла X)
/       P(Y) - П(вер-ть встретить i-ое слово из Y)
/                i=1,.., n
/       поскольку мы сравниваем две вероятности (ищем max среди них), на знаменатель можно забить и не считать его, т.к. он const
*/

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


int main() {
    int trainings, tests;
    int types0 = 0, types1 = 0;
    std::cin >> trainings >> tests;
    std::vector<data> dataBayes(trainings); //вектор файлов
    for (int i = 0; i < trainings; i++) { // обучение
        int type;
        std::cin >> type;
        if (type == 0) {
            types0++;
        } else {
            types1++;
        }
        std::string text;
        std::cin.ignore();
        getline(std::cin, text);
        dataBayes[i] = {type, makeVector(text)};
    }
    NaiveBayes classNB = NaiveBayes(types0, types1);
    classNB.initMaps(dataBayes);

    for (int i = 0; i < tests; i++) { // запросы
        std::string text;
        getline(std::cin, text);
        file doc = makeVector(text);
        std::cout << classNB.fileTypePrediction(doc) << std::endl;
        
    }
}
