#! /usr/bin/env python3

import sys
import random
import string

def main():
    str = "1 2 3 4 5 8 34  8 5 4 3  8 54 4 7 3  7 5 3 1 2 3 4 5 "
    str1 = "8 34  8 5 4 3  8 54 4 7 3 6 9  3   89 4 5 6575 2342 7 5 3 1 2 3 4 5 "
    str2 = "236 567657  234 23  78567  2 34 67 4 53  70 7 234 5235  79 876 54 432 "
    str3 = "3  8 54 4 7 3  7 5 3 1 2 3 4 5 1 2 3 4 5 8 34  8 5 4 "
    str4 = "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
    str_list = [str1, str2, str3, str4]

    if len(sys.argv) != 2:
        print(f"Usage: {0} <count_of_num_occurrences>\n".format({sys.argv[0]}))
        sys.exit(1)
    count_of_num_occurrences = int(sys.argv[1])
    count_of_lines = 1000000

    import random
    num_of_unic_occurrences = random.sample(range(1, count_of_lines), count_of_num_occurrences)
    num_of_unic_occurrences.sort()
    
    name_of_bench_file = "benchmark_test"
    test_file_name = f"{name_of_bench_file}"
    with open( f"{name_of_bench_file}", 'w' ) as test_file:
        test_file.write(f"{str}\n")
        for i in range( count_of_lines ):
            if i in num_of_unic_occurrences:
                test_file.write(f"{str}\n")
            else:
                random_str_num = random.randint(0, len(str_list) - 1)
                test_file.write(f"{str_list[random_str_num]}\n")
 
main()