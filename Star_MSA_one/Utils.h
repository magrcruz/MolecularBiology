#pragma once

#include <iostream>
#include <string>

using namespace std;


void readChain(ifstream& file, string& name, string& cadena);
void printMatrix(vector<vector<int>>& matrix, string s, string t);
void printSequence(string& s, string& t);


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
    //cout << "Nombre: " << name << endl;
    //cout << "Cadena: " << cadena << endl;
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