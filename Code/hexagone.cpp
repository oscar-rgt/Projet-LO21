#include <iostream>
#include <cstring>
#include "hexagone.h"

using namespace std;




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
	case Carriere:
		return 'X';
		break;
	}
}