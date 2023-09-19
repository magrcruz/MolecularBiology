#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "Utils.h"

#define multiple 8
#define both 4
#define up 2
#define left 1

using namespace std;

struct Alignment {
    vector<vector<int>> scores;
    vector<vector<char>> origin;
    vector<pair<string, string>> sequences;//Para almacenar secuencias ya alineadas
    string s, t;
    int n, m;
    int score;

    Alignment() { reset(); }
    Alignment(string _s, string _t);

    void reset();
    void setUp(string _s, string _t);
    void prinScoreMatrix();
    void prinOriginMatrix();

};

Alignment::Alignment(string _s, string _t) {
    setUp(_s, _t);
}
void Alignment::reset() {
    scores.clear();
    origin.clear();
    sequences.clear();
    s = t = "";
    n = m = score = 0;
}
void Alignment::setUp(string _s, string _t) {
    reset();
    s = "-" + _s;
    t = "-" + _t;
    if (s.size() < t.size()) swap(s, t);

    n = s.size();
    m = t.size();
    score = 0;
    origin = vector<vector<char>>(n, vector<char>(m));
    scores = vector<vector<int>>(n, vector<int>(m));
}

void Alignment::prinScoreMatrix(){
    printMatrix(scores, s, t);
}
void Alignment::prinOriginMatrix() {
    cout << setw(5) << "s\\t |";
    for (int i = 0; i < t.size(); i++) cout << setw(4) << t[i];
    cout << endl;
    for (int i = 0; i < origin.size(); i++) {
        cout << setw(2) << s[i] << "  |";
        for (int j = 0; j < origin[0].size(); j++) {
            cout << ((origin[i][j] & multiple) ? 'm' : '_') 
                << ((origin[i][j] & both) ? 'b' : '_')
                << ((origin[i][j] & up) ? 'u' : '_')
                << ((origin[i][j] & left) ? 'l' : '_')
                << ' ';
        }
        cout << endl;
    }
    cout << endl;

}
