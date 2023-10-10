#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

double random(double min, double max)
{
    return (double)(rand())/RAND_MAX*(max - min) + min;
}

int main() {
    int n;
    cin >> n;
    ofstream out;

    out.open("test" + to_string(n) + ".txt");
    out << n << "\n";
    for (int i = 0; i < n; i++) {
        out << random(1.0, (double) n) << " ";
    }
    out << "\n";
    for (int i = 0; i < n; i++) {
        out << random(1.0, (double) n) << " ";
    }
}