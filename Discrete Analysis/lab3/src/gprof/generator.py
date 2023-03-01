#! /usr/bin/env python3

import sys
import random
import string

def get_random_key():
    return random.choice( string.ascii_letters )
    
'''def removekey(d, key):
    r = dict(d)
    del r[key]
    return r'''

def main():
    if len(sys.argv) != 3:
        print(f"Usage: {0} <test dir> <counting of tests>\n".format({sys.argv[0]}))
        sys.exit(1)
    test_dir = sys.argv[1]
    count_of_tests = int(sys.argv[2])

    actions = [ "+", "?", "-" ]

    for enum in range( count_of_tests ):
        keys = dict()
        test_file_name = "tests/{:02d}".format( enum + 1 )
        with open( "{0}.t".format( test_file_name ), 'w' ) as test_file, \
             open( "{0}.a".format( test_file_name ), "w" ) as answer_file:

            for _ in range( random.randint(1, 10) ):
                action = random.choice( actions )
                if action == "+":
                    key = get_random_key()
                    value = random.randint(1, 100)
                    test_file.write("+ {0} {1}\n".format( key, value ))
                    key = key.lower()
                    
                    answer = "Exist"
                    if key not in keys:
                        answer = "OK"
                        keys[key] = value
                    answer_file.write( "{0}\n".format( answer ) )

                elif action == "?":
                    search_exist_element = random.choice([True, False])
                    
                    if search_exist_element and len(keys.keys()) > 0:
                        key = random.choice([key for _ in keys.keys() ]) 
                    #                        ^выбрать ключ key рандомно, перебирая все ключи в словаре
                    else :
                        key = get_random_key()

                    test_file.write("{0}\n".format(key))
                    key = key.lower()
                    if key in keys:
                        answer = "OK: {0}".format(keys[key])
                    else:
                        answer = "NoSuchWord"
                    answer_file.write("{0}\n".format(answer))

                elif action == "-":
                    remove_exist_element = random.choice([True, False])
                    
                    if remove_exist_element and len(keys.keys()) > 0:
                        key = random.choice([key for _ in keys.keys() ]) 
                    #                        ^выбрать ключ key рандомно, перебирая все ключи в словаре
                    else :
                        key = get_random_key()
                    
                    test_file.write("- {0}\n".format(key))
                    key = key.lower()

                    if key in keys:
                        answer = "OK"
                        del keys[key]
                    else:
                        answer = "NoSuchWord"
                    answer_file.write("{0}\n".format(answer))

main()