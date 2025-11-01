#include <iostream>
#include <cstring>
#include "hexagone.h"

using namespace std;


Hexagone::Hexagone(Type t, int n, bool r, int e) : type(t), niveau(n), recouvert(r) {
	if (t == Place) {
		etoiles = e;
	}
}

void Hexagone::recouvrir(bool r) {
	if (est_recouvert()) {
		throw HexaException("Hexagone deja recouvert");
	}
	recouvert = r;
}

void Hexagone::affiche() {
	cout << " / \\\n| a |\n \\ /";
}