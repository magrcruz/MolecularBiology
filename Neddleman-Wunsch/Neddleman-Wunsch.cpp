#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <iomanip>

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
    stack<pair<int, int>> to_procces;
    stack<pair<string, string>> strings;
    to_procces.push(pair<int, int>(s.size() - 1, t.size() - 1));
    strings.push(pair<string, string>());

    int i = 0, j = 0, k = 0, l = 0, b = 0;
    string u, v, temp1, temp2;
    u.reserve(s.size());
    v.reserve(t.size());

    while (!to_procces.empty()) {
        u = strings.top().first;
        v = strings.top().second;

        b = 0;
        i = to_procces.top().first;
        j = to_procces.top().second;

        to_procces.pop();
        strings.pop();
        while (i && j) {
            b = 0;
            temp1 = u;
            temp2 = v;
            k = i;
            l = j;
            if (origin[i][j] & both) {
                u += s[i--];
                v += t[j--];
                b++;
            }
            if (origin[k][l] & up) {
                if (b) {
                    to_procces.push(pair<int, int>(k - 1, l));
                    strings.push(pair<string, string>(temp1 + s[k], temp2 + "-"));
                }
                else {
                    u += s[i--];
                    v += '-';
                }
                b++;
            }
            if (origin[k][l] & left) {
                if (b) {
                    to_procces.push(pair<int, int>(k, l - 1));
                    strings.push(pair<string, string>(temp1 + "-", temp2 + t[l]));
                }
                else {
                    u += "-";
                    v += t[j--];
                }
                b++;
            }
            if (!b) exit(1);
        }
        while (i > 0) {
            u += s[i--];
            v += "-";
        }
        while (j > 0) {
            v += t[j--];
            u += "-";
        }

        reverse(u.begin(), u.end());
        reverse(v.begin(), v.end());

        output.push_back(pair<string, string>(u, v));
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

            matrix[i][j] = num[0].first;
            origin[i][j] = num[0].second |
                (num[0].first == num[1].first ? num[1].second : 0) |
                (num[0].first == num[2].first ? num[2].second : 0);
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
    string s = "AAAC";
    string t = "AGC";

    vector<pair<string, string>> sequences;
    int score = needlemanWunsch(s, t, sequences);

    cout << "El score es: " << score << endl;
    for (int i = 0; i < sequences.size(); i++) {
        printSequence(sequences[i].first, sequences[i].second);
    }
}