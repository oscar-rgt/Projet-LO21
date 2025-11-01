#include <iostream>
#include <cstring>
#include "tuiles.h"
#include "hexagone.h"
using namespace std;

int main() {
	try {
		Hexagone h1(Place, 1, false, 3);
		Hexagone h2(Jardin);
		Hexagone h3(Habitation);
		Tuile t1(0, h1, h2, h3);
		Quadrillage();
	}
	catch (const HexaException& e) {
		cout << "erreur ; " << e.getInfo();
	}
	

	return 0;
}