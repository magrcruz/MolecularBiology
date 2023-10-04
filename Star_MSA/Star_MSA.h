#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <fstream>
#include <filesystem>

//Empezarlo de 0


#define qua tuple<int, int, int, int>
#define gap "-"

#define multiple 8
#define both 4
#define up 2
#define left 1

using namespace std;

ofstream output("output.txt");
void writeToFile(vector<string> seq, int score) {
    output << "Score: " << score << endl;
    for (int i = 0; i < seq.size(); i++) {
        output << seq[i] << endl;
    }
    //cout << setfill('_') <<setw(20)<<" " << endl;
}

void printOriginMatrix(string s1, string s2, vector<vector<char>> origin2);

//Paso 1 encontrar la cadena centrica
vector<vector<int>> distances;
vector<string> sequences;

int getScore(string _s, string _t) {//No guarda el origen
    _s = gap + _s;
    _t = gap + _t;
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
void getDistancesMatrix() {
    int m = sequences.size();
    distances = vector<vector<int>>(m, vector<int>(m));
    for (int i = 0; i < m; i++) {
        distances[i][i] = 0;
        for (int j = i + 1; j < m; j++) {
            distances[i][j] = distances[j][i] = getScore(sequences[i], sequences[j]);
        }
    }
}
int getMostCentricSequence() {
    //Hacer una matriz de todos contra todos y sumar
    int max = INT_MIN, index = 0;
    for (int i = 0; i < sequences.size(); i++) {
        int curr = 0;
        for (int j = 0; j < sequences.size(); j++)
            curr += distances[i][j];
        if (curr > max) {
            index = i;
            max = curr;
        }
    }
    return index;
}
void centricSequenceToFront() {
    getDistancesMatrix();
    int indexCentric = getMostCentricSequence();
    int acumulatedScore = 0;
    //Mueve la secuencia mas centrica al comienzo
    string centricSequence = sequences[indexCentric];
    sequences.erase(sequences.begin() + indexCentric);
    sequences.insert(sequences.begin(), centricSequence);
}

//Paso 2 encontrar los alineamientos
// 2.1 Encontrar la matriz de origen
int getOriginMatrix(string s, string t, vector<vector<char>>& origin) {
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

    for (int i = 0; i < n; i++) matrix[i][0] = -2 * i;
    for (int i = 0; i < m; i++) matrix[0][i] = -2 * i;

    for (int i = 1; i < n; i++) {
        for (int j = 1; j < m; j++) {
            bool algunGap = _s[i] == '-' && _t[j] == '-';
            // Si son iguales y uno es un gap 0, si no son gaps 1 caso contrario -1
            num[0] = pair<int, char>(matrix[i - 1][j - 1] +
                (s[i] == t[j] ? (_s[i] == '-'? 0 : 1) : -1), both);
            
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
    //printOriginMatrix(_s, _t, origin);//Alignment
    //printMatrix(matrix, _s, _t);
    return matrix[matrix.size() - 1][matrix[0].size() - 1];
}

void addGap(vector<string> &sequences, int index, int range) {
    if (index < 0)
        cout << "Hey" << endl;
    for (int i = 0; i < range; i++) sequences[i].insert(index, gap);
}

int sumaPares(string s, string t) {
    int out = 0;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '-' || t[i] == '-') {
            if (s[i] != t[i]) out -= 2;
        }
        else out += (s[i] == t[i] ? 1 : -1);
    }
    return out;
}
struct alineacionScore {
    vector<string> seq;
    int score = 0;
    void print() {
        cout << "Score: " << score << endl;
        for (int i = 0; i < seq.size(); i++) {
            cout << seq[i] << endl;
        }
        cout << endl;
    }
    void recalculateScore() {
        score = 0;
        for (int i = 1; i < seq.size(); i++) {
            score += sumaPares(seq[0], seq[i]);
        }
        //cout << "Score final es" << score;
    }
};
bool comparator(alineacionScore& a, alineacionScore& b) {
    return a.score > b.score;
}



vector<alineacionScore> alineaciones;

void alingSequencesRecursion(vector<string> sequences, const vector<vector<char>>& origin, const int score, int i, int j, int posi, int posj) {
    //Revisar si sequences se pasa por referencia se debe restablecer el vector sequences
    //cout << "recursion, seqSize " << sequences.size() <<" len " << len << endl;
    if (i && j) {
        if (origin[i][j] & both) {
            vector<string> sequencesb = sequences;
            alingSequencesRecursion(sequencesb, origin, score, i - 1, j - 1, posi-1, posj-1);
        }
        if (origin[i][j] & left) {
            vector<string> sequencesl = sequences;
            addGap(sequencesl, posi, sequences.size() - 1);
            alingSequencesRecursion(sequencesl, origin, score, i, j - 1, posi, posj-1);
        }
        if (origin[i][j] & up) {
            vector<string> sequencesu = sequences;
            sequencesu[sequencesu.size() - 1].insert(posj, gap);
            //addGap(sequencesl, j, sequences.size() - 1);//Probar con j
            alingSequencesRecursion(sequencesu, origin, score, i - 1, j,posi-1, posj);
        }
    }
    else {
        while (i > 0) {
            i--;
            sequences[sequences.size() - 1].insert(0, gap);
        }
        while (j > 0) {
            j--;
            addGap(sequences, 0, sequences.size() - 1);
        }
        alineaciones.push_back(alineacionScore{ sequences, score });
    }
}

void alingNSequences(int alreadyAligned = 1) {
    //Alinea la ultima con la primera anadiendo gaps a las anteriores
    //Hacerlo recursivo
    int k = 0;
    vector<vector<char>> origin;
    while (alineaciones.size() && alineaciones[0].seq.size() == alreadyAligned) {
        int old_score = alineaciones[0].score;
        vector<string> local_sequences = alineaciones[0].seq;
        local_sequences.push_back(sequences[alreadyAligned]);//Para alinear con esta secuencia
        alineaciones.erase(alineaciones.begin());//Revisar pop front
        int originScore = getOriginMatrix(local_sequences[0], local_sequences[local_sequences.size() - 1], origin);
        // << k++ << endl;
        alingSequencesRecursion(local_sequences, origin, old_score+ originScore, local_sequences[0].size() - 1, local_sequences[local_sequences.size() - 1].size() - 1, local_sequences[0].size() - 1, local_sequences[local_sequences.size() - 1].size() - 1);
    }
}

void startAlignement(vector<string> seq) {
    sequences = seq;

    centricSequenceToFront();

    alineacionScore first;
    first.seq.push_back(sequences[0]);
    first.score = 0;

    alineaciones.clear();
    alineaciones.push_back(first);

    for (int i = 1; i < sequences.size(); i++) {
        //cout << "i=" << i;
        alingNSequences(i);
    }

    for_each(alineaciones.begin(), alineaciones.end(), [](alineacionScore& a) {a.recalculateScore();});
    sort(alineaciones.begin(), alineaciones.end(), comparator);

    cout << "ALINEACIONES" << endl;
    bool print = 1;
    for (int i = 0; print && i < alineaciones.size(); i++) {
        cout << "Alineacion #" << i << endl;
        alineaciones[i].print();
    }
    alineaciones;
}

void printOriginMatrix(string s1, string s2, vector<vector<char>> origin2) {
    cout << setw(4) << "s\\t |";
    for (int i = 0; i < s2.size(); i++) cout << setw(5) << s2[i];
    cout << endl;
    for (int i = 0; i < origin2.size(); i++) {
        cout << setw(2) << s1[i] << "  | ";
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

vector<string> seqforward;
vector<string> seqbackward;

void readSequences(int n = 6) {
    ifstream file("BRCA1.txt");

    if (file.is_open()) {
        file.close();
    }
    else {
        std::cerr << "Error al abrir el archivo: " << strerror(errno) << std::endl;
    }

    string str;
    size_t ini, fin;
    seqforward.clear();
    seqbackward.clear();
    for (int i = 0; i < n; i++) {
        getline(file, str);
        ini = str.find('-') + 1;
        fin = str.find_last_of('-') - 1;
        seqforward.push_back(str.substr(ini, fin - ini));
        
        getline(file, str);
        ini = str.find('-') + 1;
        fin = str.find_last_of('-') - 1;
        seqbackward.push_back(str.substr(ini, fin - ini));
    }
}
