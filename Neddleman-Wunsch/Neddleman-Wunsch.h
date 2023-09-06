#pragma once

#include <iostream>
#include <string>
#include <vector>

#define qua tuple<int, int, int, int>

#define multiple 8
#define both 4
#define up 2
#define left 1

using namespace std;

void readChain(ifstream& file, string& name, string& cadena);
void printMatrix(vector<vector<int>>& matrix, string s, string t);
void printSequence(string& s, string& t);

void getScoreMatrix(string& s, string& t, vector<vector<int>>& matrix, vector<vector<char>>& origin);
vector<pair<string, string>> getSequences(vector<vector<char>>& origin, string s, string t);
int needlemanWunsch(string& s, string& t, vector<pair<string, string>>& sequences);

// IO

void readChain(ifstream& file, string& name, string& cadena) {
    // Leer una cadena
    string buffer(10, '\0');
    getline(file, name);//Nombre de la cadena
    char delimiter = file.peek();
    while (delimiter != '\n') {//Mientras no sea una linea vacia
        file.read(&buffer[0], 10);//Lee el numero
        if (!isdigit(buffer[8])) break; //Hubo un error y no leyo un numero
        while (delimiter != '\n') {//Ejecuta a lo mucho 6 veces
            file.read(&buffer[0], 10);//Extrae por bloques la cadena
            cadena += buffer;
            delimiter = file.get();
        }
        delimiter = file.peek();
    }
    getline(file, buffer);//Obtiene cadena vacia
    cout << "Nombre: " << name << endl;
    cout << "Cadena: " << cadena << endl;
}

void printMatrix(vector<vector<int>>& matrix, string s, string t) {
    cout << setw(4) << "s\\t |";
    for (int i = 0; i < t.size(); i++) cout << setw(4) << t[i];
    cout << endl;
    for (int i = 0; i < matrix.size(); i++) {
        cout << setw(2) << s[i] << "  |";
        for (int j = 0; j < matrix[i].size(); j++) {
            cout << setw(4) << matrix[i][j];
        }
        cout << endl;
    }
    cout << endl;
}

void printSequence(string& s, string& t) {
    cout << ">> s = " << s << endl;
    cout << "   t = " << t << endl;
    cout << endl;
}

//Needleman-Wunsch

void getScoreMatrix(string& s, string& t, vector<vector<int>>& matrix, vector<vector<char>>& origin) {
    int n, m;
    n = s.size() + 1;
    m = t.size() + 1;
    pair<int, char> num[3];//Numero y origen
    s = "-" + s;
    t = "-" + t;
    for (int i = 0; i < n; i++) matrix[i][0] = -2 * i;
    for (int i = 0; i < m; i++) matrix[0][i] = -2 * i;

    for (int i = 1; i < n; i++) {
        for (int j = 1; j < m; j++) {
            num[0] = pair<int, char>(matrix[i - 1][j - 1] + (s[i] == t[j] ? 1 : -1), both);
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
}

vector<pair<string, string>> getSequences(vector<vector<char>>& origin, string s, string t) {
    vector<pair<string, string>> output;
    stack<qua> to_procces;//Almacena estado y x,y
    int status, i, j, len;
    string u, v, tmp1, tmp2;
    u.reserve(s.size());
    v.reserve(t.size());

    to_procces.push(qua(0, s.size() - 1, t.size() - 1, 0));
    while (!to_procces.empty()) {
        qua top = to_procces.top();
        to_procces.pop();
        status = get<0>(top);
        i = get<1>(top);
        j = get<2>(top);

        while (i && j) {
            if (!status) status = (origin[i][j] & multiple ? 2 : 1);
            if (status == 1) {//Solo tiene un camino
                if (origin[i][j] & both) {
                    u += s[i--];
                    v += t[j--];
                }
                else if (origin[i][j] & up) {
                    u += s[i--];
                    v += '-';
                }
                else if (origin[i][j] & left) {
                    u += "-";
                    v += t[j--];
                }
            }
            else if (status == 2) {//Tiene mas de un camino
                if (origin[i][j] & both) {//El primer camino es diagonal
                    if (origin[i][j] & up)  to_procces.push(qua(3, i, j, u.size()));
                    if (origin[i][j] & left)  to_procces.push(qua(4, i, j, u.size()));
                    u += s[i--];
                    v += t[j--];
                }
                else { //Tiene up y left
                    to_procces.push(qua(4, i, j, u.size()));//Almacena left
                    u += s[i--];
                    v += '-';
                }
            }
            else if (status == 3) {//Va por arriba
                len = get<3>(top);
                u.resize(len);
                v.resize(len);
                u += s[i--];
                v += '-';
            }
            else if (status == 3) {//Va por izquierda
                len = get<3>(top);
                u.resize(len);
                v.resize(len);
                u += "-";
                v += t[j--];
            }
            status = 0;
        }
        while (i > 0) {
            u += s[i--];
            v += "-";
        }
        while (j > 0) {
            v += t[j--];
            u += "-";
        }

        tmp1 = u;
        tmp2 = v;
        reverse(tmp1.begin(), tmp1.end());
        reverse(tmp2.begin(), tmp2.end());

        output.push_back(pair<string, string>(tmp1, tmp2));
    }
    return output;
}

int needlemanWunsch(string& s, string& t, vector<pair<string, string>>& sequences) {
    int n, m, score;
    n = s.size() + 1;
    m = t.size() + 1;
    vector<vector<char>> origin(n, vector<char>(m));
    {
    vector<vector<int>> matrix(n, vector<int>(m));
    getScoreMatrix(s, t, matrix, origin);
    cout << "Matriz de Scores" << endl;
    printMatrix(matrix, s, t);
    score = matrix[s.size() - 1][t.size() - 1];
    }
    sequences = getSequences(origin, s, t);
    return score;
}

// Needleman Wunsch for larger chains
// Use file for stack
struct stackFile {
    int oSize = 4 * sizeof(int);//Se almacenan 4 enteros
    FILE * top;

    stackFile() {
        top = fopen("myStack.txt", "w+");
    }
    bool empty() {
        return !ftell(top);
    }
    qua pop() {
        int a[4];
        fseek(top, -oSize, SEEK_CUR);//Lo regresa para leer
        for (int i = 0; i < 4; i++)
            fread(&a[i], sizeof(int), 1, top);
        fseek(top, -oSize, SEEK_CUR);//"elimina" el ultimo elemento
        //cout << ftell(top) << endl;//Para chequear que todo bien
        return qua(a[0], a[1], a[2], a[3]);
    }
    void push(int a, int b, int c, int d) {
        fwrite(&a, sizeof(int), 1, top);//Automaticamente mueve el ptr
        fwrite(&b, sizeof(int), 1, top);
        fwrite(&c, sizeof(int), 1, top);
        fwrite(&d, sizeof(int), 1, top);
        //cout << "push"<<ftell(top) << endl;//Para chequear que todo bien
    }
};
void getSequencesSt2(vector<vector<char>>& origin, string s, string t) {
    stackFile to_procces;//Almacena estado y x,y
    ofstream output("secuencias.txt");
    int status, i, j, len;
    string u, v, tmp1, tmp2;
    u.reserve(s.size());
    v.reserve(s.size());

    to_procces.push(0, s.size() - 1, t.size() - 1, 0);
    while (!to_procces.empty()) {
        qua top = to_procces.pop();
        status = get<0>(top);
        i = get<1>(top);
        j = get<2>(top);

        while (i && j) {
            if (!status) status = (origin[i][j] & multiple ? 2 : 1);
            if (status == 1) {//Solo tiene un camino
                if (origin[i][j] & both) {
                    u += s[i--];
                    v += t[j--];
                }
                else if (origin[i][j] & up) {
                    u += s[i--];
                    v += '-';
                }
                else if (origin[i][j] & left) {
                    u += "-";
                    v += t[j--];
                }
            }
            else if (status == 2) {//Tiene mas de un camino
                if (origin[i][j] & both) {//El primer camino es diagonal
                    if (origin[i][j] & up)  to_procces.push(3, i, j, u.size());
                    if (origin[i][j] & left)  to_procces.push(4, i, j, u.size());
                    u += s[i--];
                    v += t[j--];
                }
                else { //Tiene up y left
                    to_procces.push(4, i, j, u.size());//Almacena left
                    u += s[i--];
                    v += '-';
                }
            }
            else if (status == 3) {//Va por arriba
                len = get<3>(top);
                u.resize(len);
                v.resize(len);
                u += s[i--];
                v += '-';
            }
            else if (status == 3) {//Va por izquierda
                len = get<3>(top);
                u.resize(len);
                v.resize(len);
                u += "-";
                v += t[j--];
            }
            status = 0;
        }
        while (i > 0) {
            u += s[i--];
            v += "-";
        }
        while (j > 0) {
            v += t[j--];
            u += "-";
        }

        tmp1 = '\n' + u;
        tmp2 = '\n' + v;
        reverse(tmp1.begin(), tmp1.end());
        reverse(tmp2.begin(), tmp2.end());

        output.write(&tmp1[0], tmp1.size());
        output.write(&tmp2[0], tmp2.size());
    }
}

int needlemanWunschLarger(string& s, string& t) {
    int n, m, score;
    n = s.size() + 1;
    m = t.size() + 1;
    vector<vector<char>> origin(n, vector<char>(m));
    {
        vector<vector<int>> matrix(n, vector<int>(m));
        getScoreMatrix(s, t, matrix, origin);
        score = matrix[s.size() - 1][t.size() - 1];
    }
    getSequencesSt2(origin, s, t);

    return score;
}
