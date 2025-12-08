#include <iostream>
#include <cstring>
#include "hexagone.h"
#include "tuiles.h"

using namespace std;


string Hexagone::affiche() {
	// Habitation, Marche, Caserne, Temple, Jardin, Place
	string niv = to_string(getNiveau());
	string res;
	if (place) {
		res += to_string(getEtoiles());
	}
	else {
		res += " ";
	}
	switch (type) {
	case Habitation:
		res += "H" + niv;
		break;
	case Marche:
		res += "M" + niv;
		break;
	case Caserne:
		res += "C" + niv;
		break;
	case Temple:
		res += "T" + niv;
		break;
	case Jardin:
		res += "J" + niv;
		break;
	case Carriere:
		res += "X" + niv;
		break;
	}
	return res;
}