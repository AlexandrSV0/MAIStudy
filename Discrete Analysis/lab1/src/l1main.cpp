#include "sort.hpp"

int main() {
    std::ios::sync_with_stdio(false);
	std::vector<TKeyVal> src;
	TKeyVal st;
	while (std::cin >> st.key >> st.value) {
		src.push_back(st);
	}

    if (src.size() > 0) { 
        RadixSort(src);
    }

	for (ull i = 0; i < src.size(); i++) {
		std::cout << src[i].key << '\t' << src[i].value << '\n';
	}
}

