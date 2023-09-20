#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include "Alignments.h"
#include "Utils.h"

#define qua tuple<int, int, int, int>

using namespace std;

struct SmithWaterman : public Alignment {
    SmithWaterman(string s, string t) {
        reset();
        setUp(s, t);
    };

    vector<pair<int,int>> maximuns;

    void findMaximun();

    template <class Stack>
    void getSequences(Stack& stack);
    int getScoreMatrix();

    template <class Stack>
    void getSequencesFrompair(int x, int y, Stack stack);
    
    void run();
    void runStFile();
    void runStLim();
};

void SmithWaterman::findMaximun() {
    //Encuentra los maximos de la matriz
    int max = INT_MIN;
    for (int i = 0; i < scores.size(); i++) {
        for (int j = 0; j < scores[0].size(); j++) {
            if (scores[i][j] > max) {
                maximuns.clear();
                maximuns.push_back(pair<int,int>(i, j));
                max = scores[i][j];
            }
            else if (scores[i][j] == max) {
                maximuns.push_back(pair<int,int>(i, j));
            }
        }
    }
    score = max;
}
int SmithWaterman::getScoreMatrix() {
    pair<int, char> num[3];//Numero y origen

    //Inicializa la matriz de scores
    for (int i = 0; i < n; i++) scores[i][0] = 0;
    for (int i = 0; i < m; i++) scores[0][i] = 0;

    for (int i = 1; i < n; i++) {
        for (int j = 1; j < m; j++) {
            //Ve por cual de los caminos obtuvo un mejor puntaje y lo almacena
            num[0] = pair<int, char>(scores[i - 1][j - 1] + (s[i] == t[j] ? 1 : -1), both);
            num[1] = pair<int, char>(scores[i - 1][j] - 2, up);
            num[2] = pair<int, char>(scores[i][j - 1] - 2, left);

            if (num[2].first > num[1].first) swap(num[1], num[2]);
            if (num[1].first > num[0].first) swap(num[0], num[1]);
            if (num[2].first > num[1].first) swap(num[1], num[2]);

            scores[i][j] = num[0].first;
            origin[i][j] = num[0].second;

            if (num[0].first < 0) {//No hay camino
                scores[i][j] = origin[i][j] = 0;
            }
            else if (num[0].first == num[1].first) {//Hay mas de un camino
                origin[i][j] |= num[1].second;
                if (num[1].first == num[2].first)
                    origin[i][j] |= num[2].second;
            }

            //Check ways
            ///*
            if (origin[i][j] & both && !(scores[i - 1][j - 1])) origin[i][j] -= both;//Lo quita
            if (origin[i][j] & left && !(scores[i - 1][j - 1])) origin[i][j] -= left;//Lo quita
            if (origin[i][j] & up && !(scores[i - 1][j - 1])) origin[i][j] -= up;//Lo quita
            //*/
            if ((origin[i][j] & both + origin[i][j] & left + origin[i][j] & up) > 1) origin[i][j] |= multiple;
        }
    }
    findMaximun();
    
    return scores[maximuns[0].first][maximuns[0].second];//
}


template <class Stack>
void SmithWaterman::getSequencesFrompair(int x, int y, Stack to_procces) {
    
    //Stack<tuple<int, int, int, int>> to_procces;//Almacena estado y x,y
    int status, i, j, len;
    string u, v, tmp1, tmp2;
    u.reserve(s.size());
    v.reserve(t.size());

    to_procces.push(tuple<int, int, int, int>(0, x, y, 0));//Ya no empieza desde el origen
    while (!to_procces.empty()) {
        tuple<int, int, int, int> top = to_procces.top();
        to_procces.pop();
        status = get<0>(top);
        i = get<1>(top);
        j = get<2>(top);

        while (i && j && scores[i][j]) {
            if (!status) status = (origin[i][j] & multiple ? 2 : 1);
            if (status == 1) {//Solo tiene un camino o ningun origen
                if (!origin[i][j]) {//Alli acaba esa cadena
                    if(s[i]==t[j]){
                        u += s[i];
                        v += s[i];
                    }
                    break;
                }
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
                else {
                    cout << "Error" << endl;
                }
            }
            else if (status == 2) {//Tiene mas de un camino
                if (origin[i][j] & both) {//El primer camino es diagonal
                    if (origin[i][j] & up)  to_procces.push(tuple<int, int, int, int>(3, i, j, u.size()));
                    if (origin[i][j] & left)  to_procces.push(tuple<int, int, int, int>(4, i, j, u.size()));
                    u += s[i--];
                    v += t[j--];
                }
                else { //Tiene up y left
                    to_procces.push(tuple<int, int, int, int>(4, i, j, u.size()));//Almacena left
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
        /*while (i > 0) {
            u += s[i--];
            v += "-";
        }
        while (j > 0) {
            v += t[j--];
            u += "-";
        }*/
        to_procces;
        i = j = 0;
        tmp1 = u;
        tmp2 = v;
        reverse(tmp1.begin(), tmp1.end());
        reverse(tmp2.begin(), tmp2.end());

        sequences.push_back(pair<string, string>(tmp1, tmp2));
    }
}
//*/

template <class Stack>
void SmithWaterman::getSequences(Stack &st) {
    for (int i = 0; i < maximuns.size(); i++) {
        getSequencesFrompair<Stack>(maximuns[i].first, maximuns[i].second, st);
        while (st.size()) st.pop();
    }
}


void SmithWaterman::run() {
    stack<tuple<int, int, int, int>> normalStack;
    score = getScoreMatrix();
    //prinScoreMatrix();
    //prinOriginMatrix();
    getSequences(normalStack);
}



//With stack file
struct stackFile {
    int oSize = 4 * sizeof(int);//Se almacenan 4 enteros
    FILE* stTop;

    stackFile() {
        stTop = fopen("myStack.txt", "w+");
    }
    bool empty() {
        return !ftell(stTop);
    }
    tuple<int, int, int, int> top() {
        int a[4];
        fseek(stTop, -oSize, SEEK_CUR);//Lo regresa para leer
        for (int i = 0; i < 4; i++)
            fread(&a[i], sizeof(int), 1, stTop);
        //cout << ftell(top) << endl;//Para chequear que todo bien
        return tuple<int, int, int, int>(a[0], a[1], a[2], a[3]);
    }
    qua pop() {
        int a[4];
        fseek(stTop, -oSize, SEEK_CUR);//Lo regresa para leer
        for (int i = 0; i < 4; i++)
            fread(&a[i], sizeof(int), 1, stTop);
        fseek(stTop, -oSize, SEEK_CUR);//"elimina" el ultimo elemento
        //cout << ftell(top) << endl;//Para chequear que todo bien
        return qua(a[0], a[1], a[2], a[3]);
    }
    void push(int a, int b, int c, int d) {
        fwrite(&a, sizeof(int), 1, stTop);//Automaticamente mueve el ptr
        fwrite(&b, sizeof(int), 1, stTop);
        fwrite(&c, sizeof(int), 1, stTop);
        fwrite(&d, sizeof(int), 1, stTop);
        //cout << "push"<<ftell(top) << endl;//Para chequear que todo bien
    }
    void push(tuple<int, int, int, int> tp) {
        fwrite(&get<0>(tp), sizeof(int), 1, stTop);//Automaticamente mueve el ptr
        fwrite(&get<1>(tp), sizeof(int), 1, stTop);
        fwrite(&get<2>(tp), sizeof(int), 1, stTop);
        fwrite(&get<3>(tp), sizeof(int), 1, stTop);
        //cout << "push"<<ftell(top) << endl;//Para chequear que todo bien
    }
    void clear() {
        fseek(stTop, 0, SEEK_SET);
        stTop = fopen("myStack.txt", "w+");
    }
    int size() {
        return ftell(stTop) / oSize;
    }
};

void SmithWaterman::runStFile() {
    stackFile st;
    score = getScoreMatrix();
    cout << "Numero de maximos: "<<maximuns.size() << endl;
    //prinScoreMatrix();
    //prinOriginMatrix();
    //getSequences(st);
}

//Stack with limit
template <typename T>
struct limitedStack {
    stack<T> st;
    int limit = 100, k=0;
    limitedStack(int _lim = 100) :limit(_lim) {};
    T top() { return st.top(); }
    void pop() {st.pop(); }
    int size() { return st.size(); }
    void push(T elem) {
        if (k++ < limit) st.push(elem);
    }
    bool empty() { return !st.size(); }
};

void SmithWaterman::runStLim() {
    limitedStack<tuple<int, int, int, int>> normalStack;
    score = getScoreMatrix();
    //prinScoreMatrix();
    //prinOriginMatrix();
    getSequences(normalStack);
}
