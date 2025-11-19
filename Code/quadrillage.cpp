#include <iostream>
#include <cstring>
#include "tuiles.h"
#include "hexagone.h"
#include "quadrillage.h"

using namespace std;




/* infos quadrillage :
	- une ligne a 54 char (on ajoute donc 53 pour savoir qu'on passe une ligne)
	- lignes paires en para (donc de 2,4,6) :
		- empl 0 : la lettre se trouve au char 4
		- reste : la lettre se trouve au char 4 + 14 * emplacement
	- lignes impaires en para (donc de 1,3,5) :
		- reste : la lettre se trouve au char 11 * emplacement + 1

*/




void Quadrillage() {
	cout << "         _____         _____         _____         _____         _____         _____         _____                  \n";
	cout << "        /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\          \n";
	cout << "  _____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____    \n"; //ligne 2 sur quadri, ligne 1 en para
	cout << " /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\  \n";
	cout << "/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\ \n"; //ligne 4 sur quadri, ligne 2 en para
	cout << "\\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       / \n";
	cout << " \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/  \n"; // ligne 6 sur qadri, ligne 3 en para
	cout << " /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\  \n";
	cout << "/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\ \n"; // ligne 6 sur qadri, ligne 3 en para
	cout << "\\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       / \n";
	cout << " \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/  \n"; // ligne 8 sur qadri, ligne 4 en para
	cout << " /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\  \n";
	cout << "/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\ \n"; // ligne 10 sur qadri, ligne 5 en para
	cout << "\\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       / \n";
	cout << " \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/  \n";
	cout << " /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\  \n";
	cout << "/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\ \n"; // ligne 6 sur qadri, ligne 3 en para
	cout << "\\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       / \n";
	cout << " \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/  \n";
	cout << " /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\  \n";
	cout << "/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\ \n"; // ligne 10 sur qadri, ligne 5 en para
	cout << "\\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       / \n";
	cout << " \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/  \n";
	cout << " /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\  \n";
	cout << "/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\ \n"; // ligne 10 sur qadri, ligne 5 en para
	cout << "\\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       / \n";
	cout << " \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/  \n";
}


//string entiere collée du quadrillage:
//cout << "         _____         _____         _____         _____         _____         _____         _____                  \n        /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\          \n  _____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____    \n /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\  \n/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\ \n\\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       / \n \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/  \n /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\  \n/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\ \n\\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       / \n \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/  \n /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\  \n/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\ \n\\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       / \n \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/  \n /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\  \n/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\ \n\\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       / \n \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/  \n /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\  \n/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\ \n\\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       / \n \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/  \n /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\  \n/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\ \n\\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       /     \\       / \n \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/       \\_____/  \n";



bool est_libre(int l, int c, int emp[6][4]) {
	if (emp[l][c] == 0) {
		return true;
	}
	return false;
}

bool adjacent(int l1, int l2, int c1, int c2) {
	if (abs(l1 - l2) <= 2 && abs(c1 - c2) <= 1) {
		return true;
	}
	return false;
}

bool estBienPlace(int l[3], int c[3]) {
	if (adjacent(l[0],l[1],c[0],c[1]) && adjacent(l[0], l[2], c[0], c[2]) && adjacent(l[1], l[2], c[1], c[2])) {
		return true;
	}
	return false;
}

string remplirQuadrillage(string old, int ligne[3], int colonne[3], Tuile& t, int emplacement[6][4]) {
	int i = 0, passage = 0;
	bool possible = true;
	for (int i = 0; i < 3; i++) {
		if (!est_libre(ligne[i], colonne[i], emplacement)) {
			return "Emplacement deja pris";
		}
	}
	if (possible && estBienPlace(ligne, colonne)) {
		for (int i = 0; i < 3; i++) {
			int j = 0;
			j += 117 * (ligne[i]+1) * 2;
			if ((ligne[i]+1) % 2 == 0) {
				j += 4 + 14 * colonne[i];
			}
			else {
				j += 11 + 14 * colonne[i];
			}
			old[j] = t.getHexagone(i).affiche();
			emplacement[ligne[i]][colonne[i]] = 1;
		}
	}
	return old;
}
