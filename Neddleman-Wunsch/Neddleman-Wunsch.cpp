#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include "Neddleman-Wunsch.h"

using namespace std;

int main() {
    /*
    // Prueba con las cadenas AAAC y AGC
    string s = "AAAC";
    string t = "AGC";

    if (s.size() < t.size()) swap(s, t);

    vector<pair<string, string>> sequences;
    int score = needlemanWunsch(s, t, sequences);

    cout << "El score es: " << score << endl;
    for (int i = 0; i < sequences.size(); i++) {
        printSequence(sequences[i].first, sequences[i].second);
    }
    //*/

    //Alineamiento para las cadenas Bacteria, Saar-cov e Influenza
    //In progress
    ///*
    int ncadenas = 3;
    string tmp;
    vector<string> cadenas(ncadenas);

    ifstream file("Sequencias.txt");
    if (!file) {
        std::cerr << "No se pudo abrir el archivo." << std::endl;
        return 1;
    }

    for (int i = 0; i < ncadenas; i++)
        readChain(file, tmp, cadenas[i]);

    cout << "Primer alineamiento" << endl;
    if (cadenas[0].size() < cadenas[1].size()) swap(cadenas[0], cadenas[1]);

    needlemanWunschLarger(cadenas[0], cadenas[1]);
    //*/
    /*
    vector<pair<string, string>> sequences;
    int score = needlemanWunsch(cadenas[0], cadenas[1], sequences);

    cout << "El score es: " << score << endl;
    for (int i = 0; i < sequences.size(); i++) {
        printSequence(sequences[i].first, sequences[i].second);
    }
    //*/
}