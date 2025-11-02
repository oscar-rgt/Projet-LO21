#include <iostream>
#include <cstring>
#include "tuiles.h"
#include "hexagone.h"

using namespace std;

Tuile::Tuile(int i, Hexagone& h1, Hexagone& h2, Hexagone& h3) : id(i), orientation(N) {
	hexagones[0] = h1;
	hexagones[1] = h2;
	hexagones[2] = h3;
}
/* infos quadrillage :
	- une ligne a 54 char (on ajoute donc 53 pour savoir qu'on passe une ligne)
	- lignes paires en para (donc de 2,4,6) :
		- empl 0 : la lettre se trouve au char 4
		- reste : la lettre se trouve au char 4 + 14 * emplacement
	- lignes impaires en para (donc de 1,3,5) :
		- reste : la lettre se trouve au char 11 * emplacement + 1 

*/

void Quadrillage() {
	cout << "         _____         _____         _____          \n";
	cout << "        /     \\       /     \\       /     \\         \n";
	cout << "  _____/       \\_____/       \\_____/       \\_____   \n"; //ligne 2 sur quadri, ligne 1 en para
	cout << " /     \\       /     \\       /     \\       /     \\  \n";
	cout << "/       \\_____/       \\_____/       \\_____/       \\ \n"; //ligne 4 sur quadri, ligne 2 en para
	cout << "\\       /     \\       /     \\       /     \\       / \n";
	cout << " \\_____/       \\_____/       \\_____/       \\_____/  \n"; // ligne 6 sur qadri, ligne 3 en para
	cout << " /     \\       /     \\       /     \\       /     \\  \n";
	cout << "/       \\_____/       \\_____/       \\_____/       \\ \n"; // ligne 6 sur qadri, ligne 3 en para
	cout << "\\       /     \\       /     \\       /     \\       / \n";
	cout << " \\_____/       \\_____/       \\_____/       \\_____/  \n"; // ligne 8 sur qadri, ligne 4 en para
	cout << " /     \\       /     \\       /     \\       /     \\  \n"; 
	cout << "/       \\_____/       \\_____/       \\_____/       \\ \n"; // ligne 10 sur qadri, ligne 5 en para
	cout << "\\       /     \\       /     \\       /     \\       / \n";
	cout << " \\_____/       \\_____/       \\_____/       \\_____/  \n";
}

//string entiere collée du quadrillage:
// cout << "         _____         _____         _____\n        /     \\       /     \\       /     \\\n  _____/       \\_____/       \\_____/       \\_____\n /     \\       /     \\       /     \\       /     \\\n/       \\_____/       \\_____/       \\_____/       \\\n\\       /     \\       /     \\       /     \\       /\n \\_____/       \\_____/       \\_____/       \\_____/\n /     \\       /     \\       /     \\       /     \\\n/       \\_____/       \\_____/       \\_____/       \\\n\\       /     \\       /     \\       /     \\       /\n \\_____/       \\_____/       \\_____/       \\_____/\n /     \\       /     \\       /     \\       /     \\\n/       \\_____/       \\_____/       \\_____/       \\\n\\       /     \\       /     \\       /     \\       /\n \\_____/       \\_____/       \\_____/       \\_____/\n";

int place_libre(int ligne, int emplacement[6][4]) { //check si la place est libre (donc l'hexa en haut, a droite, a gauche)
	int res= -1;
	bool pair = (ligne+1) % 2 == 0;
	for (int i = 0; i < 4; i++) {
		if (emplacement[ligne][i] == 0 && emplacement[ligne+1][i-1] == 0 && emplacement[ligne + 2][i] == 0 && pair) {
			return i;
		}
		else if (emplacement[ligne][i] == 0 && emplacement[ligne+1][i] == 0 && emplacement[ligne + 2][i] == 0 && !pair) {
			return i;
		}
	}
	return res;
}


string remplirQuadrillage(string old, int ligne, Tuile& t, int emplacement[6][4]) {
	int i = 0, passage = 0;

	switch (t.getOrientation()) {
	case N:
		int empl = place_libre(ligne-1, emplacement);
		if (empl != -1) {
			i += 53 * (ligne * 2); // on passe ligne * 2 de lignes
			if (ligne % 2 == 0) {
				i += 4;
				i += 14 * empl;
				old[i] = t.getHexagone(0).affiche();
				while (old[i] != '\n') {
					i++;
				}
				i += 53 + 11 * (empl+1);
				old[i] = t.getHexagone(1).affiche();
				while (old[i] != '\n') {
					i++;
				}
				i += 53 + 4 + 14 * empl;
				old[i] = t.getHexagone(2).affiche();

			}
			else {
				i += 11 * (empl+1);
				old[i] = t.getHexagone(0).affiche();
				while (old[i] != '\n') {
					i++;
				}
				i += 53 + 5 + 14*(empl);
				old[i] = t.getHexagone(1).affiche();
				while (old[i] != '\n') {
					i++;
				}
				i += 53 + 12 * (empl + 1);
				old[i] = t.getHexagone(2).affiche();
			}

		}
	}
	return old;
}



