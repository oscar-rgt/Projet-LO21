#include <iostream>
#include <cstring>
#include "hexagone.h"

using namespace std;


Hexagone::Hexagone(Type t, int n, bool r, int e) : type(t), niveau(n), recouvert(r), etoiles(0) {
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

char Hexagone::affiche() {
	// Habitation, Marche, Caserne, Temple, Jardin, Place
	switch (type) {
	case Habitation:
		return 'H';
		break;
	case Marche:
		return 'M';
		break;
	case Caserne:
		return 'C';
		break;
	case Temple:
		return 'T';
		break;
	case Jardin:
		return 'J';
		break;
	case Place:
		return '0' + getEtoiles();
		break;
	}
}