#include <iostream>
#include <chrono>
#include <vector>
#include <cstdint>
#include <string>
#include <algorithm>
#include <unordered_map>

const int UNDEFINED = -1;

void ParseStrToVec(const std::string& line, std::vector<int>& vec) {
	int tmp = 0;
	bool isSpaces = true;
	for (char c : line) {
		if ('0' <= c && c <= '9') {
			tmp = tmp * 10 + c - '0';
			isSpaces = false;
		}
		else {
			if (!isSpaces) {
				vec.push_back(tmp);
				tmp = 0;
			}
			isSpaces = true;
		}
	}
	if (!isSpaces) {
		vec.push_back(tmp);
	}
}

std::unordered_map<int, std::vector<int>> PFunction(const std::vector<int> pattern) {
    std::unordered_map<int, std::vector<int>> p_func;
//    std::vector<std::vector<int>> p_func(10, std::vector<int>());
    int n = pattern.size();
    for (int i = 0; i < n; i++) {
        p_func[pattern[i]].push_back(i);
    }   
    return p_func;
}

int UseRuleBadLetter(std::unordered_map<int, std::vector<int>>& p_func, int letter, int ind_patt, int patt_size) { // p-функция, несовпавшая цифра _текста_, индекс в паттерне, размер паттерна
    auto it = p_func.find(letter);
    if (it == p_func.end()) {
        return ind_patt;
    }

    const std::vector<int> ind = it->second;
    auto it_bound = std::lower_bound(ind.begin(), ind.end(), ind_patt);
    return ind_patt - *(--it_bound);
}


std::vector<int> ZFunction(const std::vector<int>& pattern) {//вычисляет Z-функцию
	int n = pattern.size();
	std::vector<int> z_func(n, 0);
	int l = 0;
	int r = 0;
	for (int i = 1; i < n; ++i) {
		if (i <= r) {
			z_func[i] = std::min(r - i + 1, z_func[i - l]);
		}
		while (i + z_func[i] < n && pattern[z_func[i]] == pattern[i + z_func[i]]) {
			++z_func[i];
		}
		if (i + z_func[i] - 1 > r) {
			l = i;
			r = i + z_func[i] - 1;
		}
	}
	return z_func;
}

std::vector<int> NFunction(std::vector<int> pattern) {//вычисляет N-функцию
    std::reverse(pattern.begin(), pattern.end());
    int n = pattern.size();
    std::vector<int> n_func(n);
    std::vector<int> z_func = ZFunction(std::move(pattern));
    for (int i = 1; i < n; i++) {
        if (z_func[i] != 0) {
            n_func[n - i-1] = z_func[i];
        }
    }
    return n_func;
}

std::vector<int> LFunction(const std::vector<int>& pattern,std::vector<int>& n_func, int& gp_size) {//вычисляет L-функцию
    gp_size = 0;
    int n = pattern.size();
    n_func = NFunction(std::move(pattern));
    std::vector<int> l_func(n, UNDEFINED);
    for (int i = 0; i < n; i++) {
        int j = n - n_func[i];
        if (j != n) {
            l_func[j] = i;
            if (i == n - j -1) {
                gp_size = i+1;
            }
        }
    }
    return l_func;
}

int UseRuleGoodSuff(std::vector<int>& l_func, int ind_patt, int patt_size, int gp_size) {
    if (ind_patt == patt_size) {
        return 1;
    }
    if (l_func[ind_patt] == UNDEFINED) {
	//	std::cout << "in == UNDEF: PATTSIZE = " << patt_size << " GP_SIZE = " << gp_size <<'\n';
        return patt_size - gp_size;
    }
    return patt_size -1 - l_func[ind_patt];
}



int main() {
    std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	std::string line;
	getline(std::cin, line);
	std::vector<int> pattern;
	ParseStrToVec(std::move(line), pattern);
	int n = pattern.size();
//    std::cout << "pattsize = " << pattern.size() << '\n';

    int word_count = 0;
	std::vector<int> words_in_line;
	std::vector<int> text;
	while (getline(std::cin, line)) {
		ParseStrToVec(std::move(line), text);
		word_count += text.size() - word_count;
		words_in_line.push_back(word_count);
	}
    int gp_size = 0;
	bool equality = false;
    std::unordered_map<int, std::vector<int>> p_func = PFunction(pattern);
	std::vector<int> n_func;
    std::vector<int> l_func = LFunction(pattern, n_func, gp_size);
	std::vector<int> m_func(text.size(), UNDEFINED);
	auto start = std::chrono::steady_clock::now();
    for (int k = pattern.size() - 1; k < text.size();) {
		int i = k;
		bool isOk = true;
		for (int j = pattern.size() - 1; j >= 0; ) {

			if (m_func[i] == UNDEFINED || (m_func[i] == 0 && n_func[j] == 0)) {
				if (text[i] != pattern[j]) { // если несовпадение
					int offset = std::max(UseRuleBadLetter(p_func, text[i], j, pattern.size()), UseRuleGoodSuff(l_func, j+1, pattern.size(),gp_size));
					offset = std::max(offset, 1);
					m_func[k] = k - i;
					k += offset;
					isOk = false;
					
					break;
				}
				--i; // если символы совпадают, декремент. позиции текста и паттерна
				--j;
			} else if (m_func[i] < n_func[j]) { // если суфф в М меньше суфф в N, то сдвиг относительно тек. позиции на значение в М[i]
				j -= m_func[i];				
				i -= m_func[i];
			} else if (m_func[i] == n_func[j]) {
				if (n_func[j] == j+1) { // если до конца (т.е. начала) паттерна расстояние такое же, как размер суффикса, то вхождение
					isOk = true;
					equality = true;
				}
				m_func[k] = k - i;
				j -= m_func[i];
				i -= m_func[i];

			} else if (m_func[i] > n_func[j]) {
				if (n_func[j] == j+1) {
					isOk = true;
					equality = true;
					m_func[k] = k - i;
					i -= m_func[i];
					j -= n_func[j]; // Было j -= m_func[i]. Если будет не работать, попробовать j = -1;
				} else { // иначе если м_функ > n_func, то гарантированно несовпадение, и сдвиг по Бойеру-Муру
					m_func[k] = k - i;
					j -= n_func[j];
					i -= n_func[j];
					if (text[i] != pattern[j]) { // если несовпадение
						int offset = std::max(UseRuleBadLetter(p_func, text[i], j, pattern.size()), UseRuleGoodSuff(l_func, j+1, pattern.size(),gp_size));
						offset = std::max(offset, 1);
						k += offset;
						isOk = false;
						break;
					}
				}
			}
		}
		if (isOk) {
			if (!equality) {
				m_func[k] = pattern.size() - 1;
			}
			equality = false;
			int ind = k - pattern.size() + 1;
			auto it = std::upper_bound(words_in_line.begin(), words_in_line.end(), ind);
			std::cout << distance(words_in_line.begin(), it) + 1 << ", ";
			if (it == words_in_line.begin()) {
				std::cout << ind + 1;
			}
			else {
				std::cout << ind + 1 - *prev(it);
			}
			std::cout << '\n';
			k += pattern.size() - gp_size;
		//	std::cout << "new k = " << k <<'\n';
		}
	}
    auto finish = std::chrono::steady_clock::now();
	auto dur = finish - start;
	std::cerr << "AD_search: " << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count() << " ms" << '\n';

	start = std::chrono::steady_clock::now();
	auto pos_it = text.begin();
	while (pos_it != text.end()) {
		pos_it = find(pos_it, text.end(), pattern[0]);
		if ((size_t)distance(pos_it, text.end()) < pattern.size()) {
			break;
		}

		size_t from = distance(text.begin(), pos_it);
		bool isOK = true;
		for (size_t i = from; i < std::min(text.size(), from + pattern.size()); ++i) {
			if (text[i] != pattern[i - from]) {
				isOK = false;
				++pos_it;
				break;
			}
		}
		if (!isOK) {
			continue;
		}

		int idx = from;
		auto it = upper_bound(words_in_line.begin(), words_in_line.end(), idx);
		//cout << distance(words_in_line.begin(), it) + 1 << ", ";
		if (it == words_in_line.begin()) {
			//cout << idx + 1;
		}
		else {
			//cout << idx + 1 - *prev(it);
		}
		//cout << "\n";
		++pos_it;
	}
	finish = std::chrono::steady_clock::now();
	dur = finish - start;
	std::cerr << "find_search: " << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count() << " ms" << '\n';

	return 0;
}