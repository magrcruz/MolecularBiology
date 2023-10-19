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

    cout << endl << "======== BCRA ========" << endl;
    readSequences();

    cout << endl << "==== FORWARD ====" << endl;
    Star_MSA forward;
    forward.sequences = seqforward;
    forward.run();

    cout << endl << "==== BACKWARD ====" << endl;
    Star_MSA backward;
    backward.sequences = seqbackward;
    backward.run();

    cout << endl << "==== BOTH ====" << endl;
    vector<string> seqboth(seqforward);
    seqboth.insert(seqboth.end(), seqbackward.begin(), seqbackward.end());
    Star_MSA sboth;
    sboth.sequences = seqboth;
    sboth.run();
}