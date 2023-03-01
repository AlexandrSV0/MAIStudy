#include "sort.hpp"

using duration_t = std::chrono::microseconds;
const std::string DURATION_PREFIX = "us";
const int LENGTH_OF_STRING = 2048;

int main() {
    std::ios_base::sync_with_stdio(false);

    std::vector<TKeyVal> v_Radix, v_Sort;
	TKeyVal st;

	while (std::cin >> st.key >> st.value) {
		v_Radix.push_back(st);
	}
    v_Sort = v_Radix;

    std::cout << "Count of lines is " << v_Radix.size() << std::endl;
    if (v_Radix.size() == 0) {
        return 0;
    }
    std::chrono::time_point<std::chrono::system_clock> start_ts = std::chrono::system_clock::now();
    RadixSort(v_Radix);
    auto end_ts = std::chrono::system_clock::now();
    uint64_t counting_sort_ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();

    start_ts = std::chrono::system_clock::now();
    std::sort(v_Sort.begin(), v_Sort.end());
    end_ts = std::chrono::system_clock::now();
    uint64_t stl_sort_ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();/**/

    std::cout << "Counting sort time: " << counting_sort_ts << DURATION_PREFIX << std::endl;
    std::cout << "STL Sort time: " << stl_sort_ts << DURATION_PREFIX << std::endl;/**/
    return 0;
}