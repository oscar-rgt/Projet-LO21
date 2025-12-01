#include <iostream>
#include <cstring>
#include"tuiles.h"
#include "hexagone.h"
#include "cite.h"

using namespace std;



class Quadrillage {
private:
	friend class Cite;
	void StringQuadri();
	string remplirQuadrillage(string old, int ligne[3], int colonne[3], Tuile& t, int emplacement[6][4]);
	bool estBienPlace(int l[3], int c[3]);
	bool est_libre(int l, int c, int emp[6][4]);
	bool adjacent(int l1, int l2, int c1, int c2);
};
