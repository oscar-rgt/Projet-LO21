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

string Hexagone::affiche() {
	// Habitation, Marche, Caserne, Temple, Jardin, Place
	string niv = to_string(getNiveau());
	switch (type) {
	case Habitation:
		return " H" + niv;
		break;
	case Marche:
		return " M"+niv;
		break;
	case Caserne:
		return " C" + niv;
		break;
	case Temple:
		return " T" + niv;
		break;
	case Jardin:
		return " J" + niv;
		break;
	case Place:
		return to_string(getEtoiles()) + "P" + niv;
		break;
	case Carriere:
		return " X" + niv;
		break;
	}
}