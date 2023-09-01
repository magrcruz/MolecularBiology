#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <iomanip>
#include <fstream>

#define qua tuple<int, int, int, int>

#define multiple 8
#define both 4
#define up 2
#define left 1

using namespace std;

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

vector<pair<string, string>> getSequences(vector<vector<int>>& matrix, vector<vector<int>>& origin, string s, string t) {
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
                else{ //Tiene up y left
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
    int n, m;
    pair<int, int> num[3];//Numero y origen
    s = "-" + s;
    t = "-" + t;

    n = s.size();
    m = t.size();
    vector<vector<int>> matrix(n, vector<int>(m));
    vector<vector<int>> origin(n, vector<int>(m));

    for (int i = 0; i < n; i++) matrix[i][0] = -2 * i;
    for (int i = 0; i < m; i++) matrix[0][i] = -2 * i;

    for (int i = 1; i < n; i++) {
        for (int j = 1; j < m; j++) {
            num[0] = pair<int, int>(matrix[i - 1][j - 1] + (s[i] == t[j] ? 1 : -1), both);
            num[1] = pair<int, int>(matrix[i - 1][j] - 2, up);
            num[2] = pair<int, int>(matrix[i][j - 1] - 2, left);

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

    cout << "Matriz de Scores" << endl;
    printMatrix(matrix, s, t);

    sequences = getSequences(matrix, origin, s, t);
    return matrix[s.size() - 1][t.size() - 1];
}


void printSequence(string& s, string& t) {
    cout << ">> s = " << s << endl;
    cout << "   t = " << t << endl;
    cout << endl;
}

int main() {
    string s = "GTCGACGCAAGAG";
    string t = "GATTACAGAGA";

    if (s.size() < t.size()) swap(s, t);

    vector<pair<string, string>> sequences;
    int score = needlemanWunsch(s, t, sequences);

    cout << "El score es: " << score << endl;
    for (int i = 0; i < sequences.size(); i++) {
        printSequence(sequences[i].first, sequences[i].second);
    }

    //Alineamiento para las cadenas Bacteria, Saar-cov e Influenza
    //In progress
    int ncadenas = 3;
    string aux;
    vector<string> cadenas(ncadenas);
    ifstream file("Sequencias.txt");
    for (int i = 0; i < ncadenas; i++) {
        getline(cin, aux);
        cout << "Storing" << aux << endl;

    }


}