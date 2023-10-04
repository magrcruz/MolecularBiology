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

    string seq[5] = {
        "ATTGCCATT",
        "ATGGCCATT",
        "ATCCAATTTT",
        "ATCTTCTT",
        "ACTGACC"
    };
    vector<string> my_sequences(seq, seq+5);
    startAlignement(my_sequences);

    //BRCA1
    readSequences();

    startAlignement(seqforward);
    startAlignement(seqbackward);

    vector<string> seqboth(seqforward);
    seqboth.insert(seqboth.end(), seqbackward.begin(), seqbackward.end());
    startAlignement(seqboth);
}