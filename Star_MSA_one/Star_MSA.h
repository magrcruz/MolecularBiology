#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include "Alignments.h"
#include "Utils.h"

#define qua tuple<int, int, int, int>

using namespace std;

struct Star_MSA : public Alignment {
    Star_MSA() {
        reset();
        setUp("", "");
    };

    vector<vector<int>> distances;
    vector<string> sequences;

    int getScore(string _s, string _t);

    void getDistancesMatrix();
    int getMostCentricSequence();
    int getOriginMatrix(string s, string t, vector<vector<char>>& origin);

    void alingSequences(vector<vector<char>>& origin, string &s, string& t, int range);
    void addGap(int index, int range);

    void run();
};

int Star_MSA::getScore(string _s, string _t) {//No almacena la matriz
    _s = "-" + _s;
    _t = "-" + _t;
    if (_s.size() < _t.size()) swap(_s, _t);

    int n = _s.size();
    int m = _t.size();
    vector<vector<int>> scores = vector<vector<int>>(n, vector<int>(m));

    pair<int, char> num[3];//Numero y origen

    //Inicializa la matriz de scores
    for (int i = 0; i < n; i++) scores[i][0] = -2 * i;
    for (int i = 0; i < m; i++) scores[0][i] = -2 * i;

    for (int i = 1; i < n; i++) {
        for (int j = 1; j < m; j++) {
            //Ve por cual de los caminos obtuvo un mejor puntaje y lo almacena
            num[0] = pair<int, char>(scores[i - 1][j - 1] + (_s[i] == _t[j] ? 1 : -1), both);
            num[1] = pair<int, char>(scores[i - 1][j] - 2, up);
            num[2] = pair<int, char>(scores[i][j - 1] - 2, left);

            if (num[2].first > num[1].first) swap(num[1], num[2]);
            if (num[1].first > num[0].first) swap(num[0], num[1]);
            if (num[2].first > num[1].first) swap(num[1], num[2]);

            scores[i][j] = num[0].first;

        }
    }
    return scores[n - 1][m - 1];//
}

void Star_MSA::getDistancesMatrix() {
    int m = sequences.size();
    distances = vector<vector<int>>( m, vector<int>(m) );
    for (int i = 0; i < m; i++) {
        distances[i][i] = 0;
        for (int j = i+1; j < m; j++) {
            distances[i][j] = distances[j][i] = getScore(sequences[i], sequences[j]);
        }
    }
    //printMatrix(distances, "12345", "12345");
}

int Star_MSA::getMostCentricSequence() {
    //Hacer una matriz de todos contra todos y sumar
    int max=INT_MIN, index=0;
    for (int i = 0; i < sequences.size(); i++) {
        int curr = 0;
        for (int j = 0; j < sequences.size(); j++) {
            curr += distances[i][j];
        }
        if (curr > max) {
            index = i;
            max = curr;
        }
    }
    return index;
}

void Star_MSA::addGap(int index, int range) {
    for (int i = 0; i < range; i++) sequences[i].insert(index,"-");
}

void Star_MSA::alingSequences(vector<vector<char>>&origin, string &s, string &t, int range) {
    int i, j;
    string u, v, _s = '-'+s, _t= '-' + t;
    u.reserve(_s.size());
    v.reserve(_s.size());

    i = _s.size()-1;
    j = _t.size()-1;

    while (i && j) {//La secuencia ya no tiene gap al comienzo
        if (origin[i][j] & both) {
            u += _s[i--];
            v += _t[j--];
        }
        else if (origin[i][j] & up) {
            u += _s[i--];
            v += '-';
        }
        else if (origin[i][j] & left) {
            addGap(s.size()-u.size(), range);//Inserta gaps en todas las secuencias
            u += "-";
            v += _t[j--];
        }
        //printSequence(u, v);
    }
    while (i > 0) {
        u += _s[i--];
        v += "-";
    }
    while (j > 0) {
        v += _t[j--];
        addGap(s.size() - u.size(), range);
        u += "-";
    }
    reverse(u.begin(), u.end());
    reverse(v.begin(), v.end());

    s = u;
    t = v;
}

void Star_MSA::run() {
    getDistancesMatrix();
    int indexCentric = getMostCentricSequence();
    int acumulatedScore = 0;
    //Mueve la secuencia mas centrica al comienzo
    string centricSequence = sequences[indexCentric];
    sequences.erase(sequences.begin() + indexCentric);
    sequences.insert(sequences.begin(), centricSequence);

    for (int i = 1; i < sequences.size(); i++) {
        vector<vector<char>> origin;
        int score = getOriginMatrix(sequences[0], sequences[i], origin);//0 porque ahora alli esta la secuencia centrica
        acumulatedScore += score;
        alingSequences(origin, sequences[0], sequences[i], i);
    }
    cout << "Alineamientos:" << endl;
    for (int i = 0; i < sequences.size(); i++)
        cout << setw(4)<<i<<"  " << sequences[i] << endl;
    cout << "Score acumulado: " << acumulatedScore;

    cout << endl;
}


int Star_MSA::getOriginMatrix(string s, string t, vector<vector<char>>& origin) {
    int n, m;
    string _s = s, _t = t;
    //if (_s.size() < _t.size()) swap(_s, _t);//No es necesario
    _s = '-' + _s;
    _t = '-' + _t;

    n = s.size() + 1;
    m = t.size() + 1;

    vector<vector<int>> matrix(n, vector<int>(m));
    origin = vector<vector<char>>(n, vector<char>(m));

    pair<int, char> num[3];//Numero y origen
    s = "-" + s;
    t = "-" + t;
    for (int i = 0; i < n; i++) matrix[i][0] = -2 * i;
    for (int i = 0; i < m; i++) matrix[0][i] = -2 * i;

    for (int i = 1; i < n; i++) {
        for (int j = 1; j < m; j++) {
            // Condicion extra por si uno de ello es un gap, no agrega ni quita
            if (_s[i] == '-' || _t[j] == '-') num[0] = pair<int, char>(matrix[i - 1][j - 1], both);
            else num[0] = pair<int, char>(matrix[i - 1][j - 1] + (s[i] == t[j] ? 1 : -1), both);

            num[1] = pair<int, char>(matrix[i - 1][j] - 2, up);
            num[2] = pair<int, char>(matrix[i][j - 1] - 2, left);

            if (num[2].first > num[1].first) swap(num[1], num[2]);
            if (num[1].first > num[0].first) swap(num[0], num[1]);
            if (num[2].first > num[1].first) swap(num[1], num[2]);

            matrix[i][j] = num[0].first;
            origin[i][j] = num[0].second;

            if (num[0].first == num[1].first) {//Hay mas de un camino
                origin[i][j] |= multiple | num[1].second;
                if (num[1].first == num[2].first)
                    origin[i][j] |= num[2].second;
            }
        }
    }
    prinOriginMatrix(s, t, origin);
    return matrix[matrix.size() - 1][matrix[0].size() - 1];
}