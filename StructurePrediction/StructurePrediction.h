#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

#define multiple 8
#define both 4
#define up 2
#define left 1

#define valEmpty 100

using namespace std;

string R;
int n;
int t; //Umbral de distancia

void setString(string str) {
	R = str;
	n = R.size();
}
struct Position {
	int x, y;
	Position(int _x, char _y) : x(_x), y(_y) {};
};

ostream& operator << (ostream& o, const Position&p) {
	o << setw(2) << p.x << "," << setw(2) << p.y;
	return o;
}

vector<vector<int>> energyMatrix;
vector<vector<vector<Position>>> originMatrix;

void printMatix() {
	cout << "________________________________________" << endl<<"   ";
	for (int i = 0; i < energyMatrix.size(); i++) 
		cout << setw(3) << R[i];
	cout << endl;
	for (int i = 0; i < energyMatrix.size(); i++) {
		cout << setw(3) << R[i];
		for (int j = 0; j < energyMatrix[0].size(); j++) {
			cout << setw(3) << energyMatrix[i][j];
		}
		cout << endl;
	}
	cout << endl;
}
void printOrigin() {
	cout << "________________________________________" << endl << "   ";
	for (int i = 0; i < originMatrix.size(); i++)
		cout << setw(3) << R[i];
	cout << endl;
	for (int i = 0; i < originMatrix.size(); i++) {
		cout << setw(3) << R[i];
		for (int j = 0; j < originMatrix[0].size(); j++) {
			if (!originMatrix[i][j].size()) cout << " None ";
			else if (originMatrix[i][j].size() == 1)
				cout << originMatrix[i][j][0] << " ";
			else cout << "More ";//More than one
		}
		cout << endl;
	}
	cout << endl;
}

struct Pairing {
	char parA, parB;
	bool emparejado = false;
	Pairing(char _parA, char _parB, bool _emparejado) :
		parA(_parA), parB(_parB), emparejado(_emparejado) {};
};
void printEmparejamiento(vector<Pairing> pares);

struct EnergyOrigin {
	int energy = INT_MAX;
	vector<Position> origin;
	EnergyOrigin() {};
	EnergyOrigin(int x, int y) {
		energy = energyMatrix[x][y];
		origin.push_back(Position(x, y));
	}
};

int alpha(int i, int j) {
	switch (R[i]){
	case 'A': return (R[j] == 'U' ? -1 : 0);
	case 'U': return (R[j] == 'A' ? -1 : 0);
	case 'C': return (R[j] == 'G' ? -1 : 0);
	case 'G': return (R[j] == 'C' ? -1 : 0);
	default: return 0;
	}
}

// Se calcula sin bucles
void calculatemin(int i, int j, EnergyOrigin& cell) {
	for (int k = i + 1; k < j; k++) {
		int currentEnergy = energyMatrix[i][k] + energyMatrix[k + 1][j];
		if (currentEnergy < cell.energy) {
			cell.energy = currentEnergy;
		}
	}
}

bool energyComparator(EnergyOrigin a, EnergyOrigin b) {
	return a.energy < b.energy;
}

void calculateEnergy(int i, int j) {
	vector<EnergyOrigin> posibilities(4);
	posibilities[0] = EnergyOrigin(i + 1, j);
	posibilities[1] = EnergyOrigin(i, j - 1);
	posibilities[2] = EnergyOrigin(i + 1, j - 1);
	posibilities[2].energy += alpha(i, j);
	//posibilities[3] = EnergyOrigin();
	calculatemin(i, j, posibilities[3]);

	sort(posibilities.begin(), posibilities.end(), energyComparator);

	//bool posibleEmpty = posibilities[0].origin.empty() ? 1 : 0;

	for (int i = 1;i < posibilities.size() &&
		posibilities[i].energy == posibilities[0].energy; i++) {

		//posibleEmpty = 0;
		posibilities[0].origin.insert(
			posibilities[0].origin.end(),
			posibilities[i].origin.begin(),
			posibilities[i].origin.end());
	}
	energyMatrix[i][j] = posibilities[0].energy;//posibleEmpty? valEmpty : posibilities[0].energy;//En el caso de que este vacio no coloca "nada" en la matriz
	originMatrix[i][j] = posibilities[0].origin;
}





vector<vector<Pairing>> output;
void reconstruct(int f, int c, vector<Pairing> pairings) {//Ver si se hacen copias
	if (f < c) {
		for (int i = 0; i < originMatrix[f][c].size(); i++) {
			int newf = originMatrix[f][c][i].x;
			int newc = originMatrix[f][c][i].y;
			if (f + 1 == newf && c - 1 == newc) {//Vino de la diagonal
				pairings.push_back(Pairing(R[f], R[c], (alpha(f, c)==-1?1:0)));
				reconstruct(newf, newc, pairings);
				pairings.erase(pairings.end() - 1);
			}
			else if (f + 1 == newf) {//Abajo
				pairings.push_back(Pairing(R[f], '-', 0));
				reconstruct(newf, newc, pairings);
				pairings.erase(pairings.end() - 1);
			}
			else if (c - 1 == newc) {//Izquierda
				pairings.push_back(Pairing('-', R[c], 0));
				reconstruct(newf, newc, pairings);
				pairings.erase(pairings.end() - 1);
			}
		}
	}
	else {
		if (f == c) {
			pairings.push_back(Pairing(R[f], '-', 0));
		}

		output.push_back(pairings);
		//cout << "hi" << endl;

		if (f == c) {
			pairings.erase(pairings.end() - 1);
		}
	}
}



void structurePrediction() {
	// Build matrix
	energyMatrix = vector<vector<int>>(n,vector<int>(n,0));//Cambiar x 0
	originMatrix = vector<vector<vector<Position>>>(n, vector<vector<Position>>(n));
	//Inicializacion, innecesaria porque ya esta la matriz
	energyMatrix[0][0] = 0;
	for (int i = 1; i < n; i++) energyMatrix[i][i] = energyMatrix[i][i - 1] = 0;

	for (int d = 1; d < n; d++) {
		for (int i = 0; i + d < n; i++) {
			int j = i + d;
			//cout << d <<" " << i << endl;
			calculateEnergy(i, j);
		}
	}

	// 
	printMatix();
	printOrigin();

	// Traceback
	vector<Pairing> temp;
	output.clear();

	reconstruct(0, originMatrix[0].size()-1, temp);

	cout << "Score: " << energyMatrix[0][originMatrix[0].size() - 1] << endl;
	cout << "EMPAREJAMIENTOS" << endl;
	for (int i = 0; i < output.size(); i++) {
		printEmparejamiento(output[i]);
		cout << endl;
	}
}

void runTest() {
	string str = "GGAAAUCC";
	setString(str);
	structurePrediction();

	str = "ACUCGAUUCCGAG";
	setString(str);
	structurePrediction();
}

/*
GC GC AU A. .A  
*
G - G - A - A - ┐
|   |   |       |
C - C - U - A - ┘
Si esta emparejado 

G - G - A - A - A
|   |           |
C - C - . - . - U
*/



vector<Pairing> paresTest;
void buildParestTest() {
	paresTest.clear();
	string toFill = "GC1GC1A-0A-0AU0";
	paresTest.reserve(toFill.size() / 3);
	for (int i = 0; i < toFill.size(); i += 3)
		paresTest.push_back(Pairing(toFill[i], toFill[i + 1], toFill[i + 2] == '1'));
}

void printEmparejamiento(vector<Pairing> pares) {
	if (!pares.size()) {
		cout << "No se encontraron emparejamientos" << endl;
		return;
	}
	//Se imprime por lineas
	//Linea 1
	if (!pares[0].emparejado) cout << ",- ";
	cout << pares[0].parA;
	for (int i = 1; i < pares.size(); i++)
		cout << " - " << pares[i].parA;
	if (!(pares[pares.size()-1].emparejado)) cout << " -,";
	cout << endl;

	//Linea 2
	cout << "|";
	if (!pares[0].emparejado) cout << "   ";
	for (int i = 1; i < pares.size(); i++)
		cout << (pares[i].emparejado? "   |" : "    ");
	if (!(pares[pares.size() - 1].emparejado)) cout << "  |";
	cout << endl;

	//Linea 3
	if (!pares[0].emparejado) cout << "'- ";
	cout << pares[0].parB;
	for (int i = 1; i < pares.size(); i++)
		cout << " - " << pares[i].parB;
	if (!(pares[pares.size() - 1].emparejado)) cout << " -'";
	cout << endl;
}

