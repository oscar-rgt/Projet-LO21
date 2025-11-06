#include <iostream>
#include <cstring>
#include"tuiles.h"
#include "hexagone.h"

using namespace std;




void Quadrillage();
string remplirQuadrillage(string old, int ligne[3], int colonne[3], Tuile& t, int emplacement[6][4]);
bool estBienPlace(int l[3], int c[3]);
bool est_libre(int l, int c, int emp[6][4]);