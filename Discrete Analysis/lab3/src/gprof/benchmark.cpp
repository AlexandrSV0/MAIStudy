#include <iostream>
#include <map>
#include <chrono>

#include "detail_avl.h"

using duration_t = std::chrono::microseconds;
using ull = unsigned long long;
int numder_of_nodes = 1000;


int main() {
    std::map<std::string, ull> rb_tree;

    TDetailAvl avl_tree;

    std::chrono::time_point<std::chrono::system_clock> start, end;
    int64_t rb_ts = 0, avl_ts = 0;

    while(numder_of_nodes <= 1000000){
        std::cout << "----------------" << "numder_of_nodes = " << numder_of_nodes << " ----------------\n";
        
    
        for (int i = 0; i <= numder_of_nodes; i++) {
            {
                start = std::chrono::system_clock::now();
                rb_tree.insert( {std::to_string(i), i} );
                end = std::chrono::system_clock::now();
                rb_ts += std::chrono::duration_cast<duration_t>( end - start ).count();
            }
            {
                start = std::chrono::system_clock::now();
                avl_tree.Add( std::to_string(i), i );
                end = std::chrono::system_clock::now();
                avl_ts += std::chrono::duration_cast<duration_t>( end - start ).count();
            }
        }
        std::cout << "Insert:\n" << "std::map ms=" << rb_ts << "\navl ms=" << avl_ts;

        rb_ts = avl_ts = 0;
        for (int i = 0; i <= numder_of_nodes; i++) {
            {
                start = std::chrono::system_clock::now();
                rb_tree.find( std::to_string(i) );
                end = std::chrono::system_clock::now();
                rb_ts += std::chrono::duration_cast<duration_t>( end - start ).count();
            }
            {
                start = std::chrono::system_clock::now();
                avl_tree.Find( std::to_string(i) );
                end = std::chrono::system_clock::now();
                avl_ts += std::chrono::duration_cast<duration_t>( end - start ).count();
            }
        }
        std::cout << "\n\nFind:\n" << "std::map ms=" << rb_ts << "\navl ms=" << avl_ts;

        rb_ts = avl_ts = 0;
        for (int i = 0; i <= numder_of_nodes; i++) {
            {
                start = std::chrono::system_clock::now();
                rb_tree.erase( std::to_string(i) );
                end = std::chrono::system_clock::now();
                rb_ts += std::chrono::duration_cast<duration_t>( end - start ).count();
                rb_tree.insert( {std::to_string(i), i} );
            }
            {
                start = std::chrono::system_clock::now();
                avl_tree.Delete( std::to_string(i) );
                end = std::chrono::system_clock::now();
                avl_ts += std::chrono::duration_cast<duration_t>( end - start ).count();
                avl_tree.Add( std::to_string(i), i );
            }
        }
        std::cout << "\n\nDelete:\n" << "std::map ms=" << rb_ts << "\navl ms=" << avl_ts << "\n\n";

        numder_of_nodes *= 10;
    }
}