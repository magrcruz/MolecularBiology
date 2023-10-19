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
    /*
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

    //Ordena las cadenas
    if (cadenas[1].size() < cadenas[2].size()) swap(cadenas[1], cadenas[2]);
    if (cadenas[0].size() < cadenas[1].size()) swap(cadenas[0], cadenas[1]);
    if (cadenas[1].size() < cadenas[2].size()) swap(cadenas[1], cadenas[2]);
    //Matriz de puntos
    pointMatrix(cadenas[0],cadenas[1]);

    cout << "Primer alineamiento" << endl;
    needlemanWunschLarger(cadenas[0], cadenas[1]);//Almacenar en los archivos con un max de 100 alineamientos
    
    cout<<"Scores"<<endl;

    vector<vector<int>> scores(3, vector<int>(3));
    for(int i=0;i<3;i++){
        for(int j=i+1;j<3;j++){
            scores[i][j]=scores[j][i]= getScoreAligned(cadenas[i],cadenas[j]);
        }
    }
    printMatrix(scores,"ABC","ABC");

    //Implementacion de penalizacion por rupturas
    getBestSecuencesSplit();
    //*/
    string s = "ATTGCCATT", t = "ATCCAATTTT";
    vector<pair<string, string>> sequences;
    needlemanWunsch(s, t, sequences);
}
