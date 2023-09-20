#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include "SmithWaterman.h"

using namespace std;

int main() {
    /*
    // Prueba con las cadenas AAAC y AGC
    {
    string s = "AAAC";
    string t = "AGC";

    SmithWaterman align(s, t);

    align.run();
    align.prinScoreMatrix();
    //align.prinOriginMatrix();

    cout << "El score es: " << align.score << endl;
    for (int i = 0; i < align.sequences.size(); i++) {
        printSequence(align.sequences[i].first, align.sequences[i].second);
    }
    }
    //*/

    //Leer cadenas Bacteria, Saar-cov e Influenza

    ///*
    int ncadenas = 3;
    string tmp;
    vector<string> cadenas(ncadenas);
    vector<string> nombres(ncadenas);
    vector<vector<int>> scores(ncadenas, vector<int>(ncadenas));

    ifstream file("Sequencias.txt");
    if (!file) {
        std::cerr << "No se pudo abrir el archivo." << std::endl;
        return 1;
    }

    for (int i = 0; i < ncadenas; i++) {
        readChain(file, tmp, cadenas[i]);
        nombres[i] = tmp;
    }

    /*
    for (int i = 0; i < ncadenas; i++) {
        scores[i][i] = 0;
        for (int j = i + 1; j < ncadenas; j++) {
            cout << "Alineando " << nombres[i] << " y " << nombres[j] << endl;
            SmithWaterman align(cadenas[i], cadenas[j]);
            align.run();
            cout << "El score es: " << align.score << endl;
            scores[i][j] = scores[j][i] = align.score;

            for (int k = 0; k < align.sequences.size(); k++) {
                printSequence(align.sequences[k].first, align.sequences[k].second);
            }
        }
    }

    cout << "Scores" << endl;
    printMatrix(scores, "ABC", "ABC");
    */

    //Obtener alineacion con menos rupturas
    {
    SmithWaterman align(cadenas[0], cadenas[1]);
    align.run();
    vector<pair<string, string>> seq = align.bestAlignments();
    cout << "Best aligments " << nombres[0] << " & " << nombres[1] << endl;
    for (int i = 0; i < seq.size(); i++) {
        printSequence(seq[i].first, seq[i].second);
    }
    }

    {
    SmithWaterman align(cadenas[1], cadenas[2]);
    align.runStLim();
    vector<pair<string, string>> seq = align.bestAlignments();
    cout << "Best aligments " <<nombres[1]<<" & "<<nombres[2]<< endl;
    for (int i = 0; i < seq.size(); i++) {
        printSequence(seq[i].first, seq[i].second);
    }
    }

    {
        SmithWaterman align(cadenas[0], cadenas[2]);
        align.runStLim();
        vector<pair<string, string>> seq = align.bestAlignments();
        cout << "Best aligments " << nombres[0] << " & " << nombres[2] << endl;
        for (int i = 0; i < seq.size(); i++) {
            printSequence(seq[i].first, seq[i].second);
        }
    }
}