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
    vector<pair<string, string>> alignments;//Para almacenar secuencias ya alineadas
    string s, t;
    int n, m;
    int score;

    Alignment() { reset(); }
    Alignment(string _s, string _t);

    void reset();
    void setUp(string _s, string _t);
    void prinScoreMatrix();
    void prinOriginMatrix(string s1 = "", string s2="", vector<vector<char>> origin = vector<vector<char>>());

    int countSplits(string aling);
    vector<pair<string, string>> bestAlignments();
};

Alignment::Alignment(string _s, string _t) {
    setUp(_s, _t);
}
void Alignment::reset() {
    scores.clear();
    origin.clear();
    alignments.clear();
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
void Alignment::prinOriginMatrix(string s1, string s2, vector<vector<char>> origin2) {
    if (!s1.size() && !s2.size() && !origin2.size()) {
        s1 = s;
        s2 = t;
        origin2 = origin;
    }
    cout << setw(4) << "s\\t |";
    for (int i = 0; i < s2.size(); i++) cout << setw(5) << s2[i];
    cout << endl;
    for (int i = 0; i < origin2.size(); i++) {
        cout << setw(2) << s1[i] << "  |";
        for (int j = 0; j < origin2[0].size(); j++) {
            cout << ((origin2[i][j] & multiple) ? 'm' : '_') 
                << ((origin2[i][j] & both) ? 'b' : '_')
                << ((origin2[i][j] & up) ? 'u' : '_')
                << ((origin2[i][j] & left) ? 'l' : '_')
                << ' ';
        }
        cout << endl;
    }
    cout << endl;

}

int Alignment::countSplits(string aling){
    int k = 0;
    bool contando = false;
    for (int i = 0; i < aling.size(); i++) {
        if (aling[i] == '-' && !contando) {
            contando = true;
            k++;
        }
        else if (contando && aling[i] != '-')
            contando = false;
    }
    if (aling[0] == '-') k--;
    if (aling[aling.size() - 1] == '-') k--;
    return k;
}

vector<pair<string, string>> Alignment::bestAlignments(){
    vector<pair<string, string>> align;
    int curr = 0, minSplit = INT_MAX;
    for (int i = 0; i < alignments.size(); i++) {
        curr = countSplits(alignments[i].first) + countSplits(alignments[i].second);
        if (curr < minSplit) {
            minSplit = curr;
            align.clear();
            align.push_back(alignments[i]);
        }
        else if(curr == minSplit) {
            align.push_back(alignments[i]);
        }
    }
    return align;
}