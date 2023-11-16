#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <stdlib.h> /* rand */

using namespace std;

//For making the profile
vector<string> sequences;
string anminoacidos = "STQYGAVLICMPFWDEKRH";

void printSequences() {
	cout << "SECUENCIAS" << endl;
	for (int i = 0; i < sequences.size(); i++) {
		cout << sequences[i] << endl;
	}
}

int denoTransicion = 0; //denominador de transicion
int denoProfile = 0;

struct Estado {
	//La probabilidad de cambiar de un estado a otro com una suma acumulada
	int pTransicionAcumulada[3] = { 0,0,0 };

	Estado* status[3];
	// #Ek -> El / #Ek -> E

	void set(int _p1, int _p2, int _p3, Estado* _e1, Estado* _e2, Estado* _e3) {
		pTransicionAcumulada[0] = _p1;
		pTransicionAcumulada[1] = _p2;
		pTransicionAcumulada[2] = _p3;

		status[0] = _e1;
		status[1] = _e2;
		status[2] = _e3;
	}

	void goToNewState(Estado *&actual) {
		int pRandom = rand() % denoTransicion;
		if (pRandom < pTransicionAcumulada[0]) actual = status[0];
		else if (pRandom < pTransicionAcumulada[1]) actual = status[1];
		else actual = status[2];
	}

	virtual void emit() {};
};

string seq;

struct Match:Estado{
	vector<pair<int,char>> profileAcumulado;//Sin el add one
	void calculateProfile(int col) {
		map<char, int> profile;

		//procentaje de observacion de la columna
		for (int i = 0; i < sequences.size(); i++) {
			//Revisa si ya calculo sobre ese valor
			char anminoacido = sequences[i][col];

			if (anminoacido != '-') {
				if (profile.find(anminoacido) == profile.end())//No lo encontro
					profile[anminoacido] = 1;
				else
					profile[anminoacido]++;
			}
		}

		for (const auto& entry : profile) {
			char tipo = entry.first;
			int cantidad = entry.second;

			profileAcumulado.push_back(pair<int, char>(cantidad, tipo));
		}

	};

	void calculateProfAcum(int col) {
		calculateProfile(col);
		for (int i = 1; i < profileAcumulado.size(); i++)
			profileAcumulado[i].first += profileAcumulado[i - 1].first;
	};
	void emit() {
		int n = rand() % (20 + sequences.size());
		if (n < 20) seq.push_back(anminoacidos[n]);
		else {
			for (int i = 0; i < profileAcumulado.size(); i++) {
				if (n <= profileAcumulado[i].first)
					seq.push_back(profileAcumulado[i].second);
			}
		}
	};
	void printProfile() {
		cout << "PROFILE" << endl;
		cout << profileAcumulado[0].second << " -> " << profileAcumulado[0].first << "/" << sequences.size() << endl;
		for (int i = 1; i < profileAcumulado.size(); i++) 
			cout << profileAcumulado[i].second << " -> " << profileAcumulado[i].first - profileAcumulado[i - 1].first << "/" << sequences.size() + 20 << endl;
		cout << "else -> 1/"<<sequences.size()+20 << endl;
	}

};

struct Insert:public Estado{
	void emit() {
		//Background
		seq.push_back(anminoacidos[rand() % 20]);
	}
};

//Delete no emite ningun simbolo

vector<Match> match;
vector<Insert> insertion;
vector<Estado> deletes;
vector<Estado> otrosEstados;//Begin y end

void calculateProbabilities(int i, int& M, int& I, int& D) {
	M = I = D = 1;//Add one
	for (int j = 0; j < sequences.size(); j++) {
		if (sequences[j][i + 1] == '-') D++;
		else M++;
	}
	cout << " M " << M << "/" << sequences.size() + 3;
	cout << " I " << I << "/" << sequences.size() + 3;
	cout << " D " << D << "/" << sequences.size() + 3;
	cout << endl;
}

void buildHMM(int col) {
	int p1, p2, p3;
	
	otrosEstados = vector<Estado>(2);//Begin y End
	match = vector<Match>(col + 3);
	insertion = vector<Insert>(col+1);
	deletes = vector<Estado>(col+1); //El primero es figurativo

	// Conecta de begin
	cout << "Transicion de begin " << " : ";
	calculateProbabilities(-1, p1, p2, p3);
	otrosEstados[0].set(p1, p2, p3, &match[1], &insertion[0], &deletes[1]);
	insertion[0].set(p1, p2, p3, &match[1], &insertion[0], &deletes[1]);//La insercion va a el mismo

	// Conecta estados intermedios
	for (int i = 1; i < col; i++) {
		cout << "Transicion en la columna " << i << " : ";
		calculateProbabilities(i, p1, p2, p3);
		match[i].set(p1, p2, p3, &match[i+1], &insertion[i], &deletes[i+1]);
		insertion[i].set(p1, p2, p3, &match[i + 1], &insertion[i], &deletes[i + 1]);
		deletes[i].set(p1, p2, p3, &match[i + 1], &insertion[i], &deletes[i + 1]);


		//Calcula el profile
		match[i].calculateProfAcum(i-1);//Porque empieza a contar desde 0
		cout << "Match " << i << " - ";
		match[i].printProfile();
		cout << endl;
	}

	// Unir a estado end
	p1 = denoTransicion-1;
	p2 = 1;

	match[col].set(p1, p2, 0, &otrosEstados[1], &insertion[col], 0);
	insertion[col].set(p1, p2, 0, &otrosEstados[1], &insertion[col], 0);
	deletes[col].set(p1, p2, 0, &otrosEstados[1], &insertion[col], 0);
}

void deleteEmptierColumns() {
	float numSequences = sequences.size();
	for (int i = 0; i < sequences.size();) {//Columna
		float notEmpty = 0;
		for (int j = 0; j < numSequences; j++) {
			if (sequences[j][i] != '-') notEmpty++;
		}
		if (notEmpty <= numSequences / float(2)) {
			//Mas de la mitad no esta vacia
			for (int j = 0; j < numSequences; j++) {
				//elimina columnas vacias
				sequences[j].erase(i, 1);
			}
		}
		else {
			i++;
		}
	}
}

int calculateNColumns() {
	return sequences[0].size();
}

void test() {
	vector<string> _sequences = {
		"VGA--HAGEY",
		"V----NVDEV",
		"VEA--DVAGH",
		"VKG------D",
		"VYS--TYETS",
		"FNA--NIPKH",
		"IAGADNGAGY",
	};

	sequences = _sequences;
	deleteEmptierColumns();
	cout << "Cantidad de estados Match: " << calculateNColumns() << endl;
	printSequences();
	cout << endl;


	buildHMM(calculateNColumns());
}