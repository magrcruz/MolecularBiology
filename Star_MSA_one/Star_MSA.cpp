#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include "Star_MSA.h"

using namespace std;

int main() {

    Star_MSA aling;

    string seq[5] = {
        "ATTGCCATT",
        "ATGGCCATT",
        "ATCCAATTTT",
        "ATCTTCTT",
        "ACTGACC"
    };
    aling.sequences = vector<string>(seq, seq+5);
    aling.run();
}