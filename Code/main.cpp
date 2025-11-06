#include <iostream>
#include <cstring>
#include "tuiles.h"
#include "hexagone.h"
#include "quadrillage.h"
using namespace std;



int main() {
	try {
		Hexagone h1(Place, 1, false, 3);
		Hexagone h2(Jardin);
		Hexagone h3(Habitation);
		Tuile t1(0, h1, h2, h3, 3);
		int emplacement[6][4] = { {0,0,0},{0,0,0,0},{0,0,0},{0,0,0,0},{0,0,0},{0,0,0,0} };
		string old = "         _____         _____         _____          \n        /     \\       /     \\       /     \\         \n  _____/       \\_____/       \\_____/       \\_____   \n /     \\       /     \\       /     \\       /     \\  \n/       \\_____/       \\_____/       \\_____/       \\ \n\\       /     \\       /     \\       /     \\       / \n \\_____/       \\_____/       \\_____/       \\_____/  \n /     \\       /     \\       /     \\       /     \\  \n/       \\_____/       \\_____/       \\_____/       \\ \n\\       /     \\       /     \\       /     \\       / \n \\_____/       \\_____/       \\_____/       \\_____/  \n /     \\       /     \\       /     \\       /     \\  \n/       \\_____/       \\_____/       \\_____/       \\ \n\\       /     \\       /     \\       /     \\       / \n \\_____/       \\_____/       \\_____/       \\_____/  \n";
		//Quadrillage();
		int tab1[3] = { 2,4,3 };
		int tab2[3] = { 1,1,2 };
		old = remplirQuadrillage(old, tab1 ,tab2, t1, emplacement);
		cout << old;
		Hexagone h11(Place, 1, false, 1);
		Hexagone h22(Temple);
		Hexagone h33(Caserne);
		Tuile t2(0, h11, h22, h33, 1);
		int tab3[3] = { 0,1,2 };
		int tab4[3] = { 0,0,1 };
		old = remplirQuadrillage(old, tab3, tab4,t2, emplacement);
		cout << old;
	}
	catch (const HexaException& e) {
		cout << "erreur ; " << e.getInfo();
	}
	

	return 0;
}