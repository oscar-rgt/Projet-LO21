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

void Hexagone::affiche(int i) {
	// Habitation, Marche, Caserne, Temple, Jardin, Place
	string s = "";
	for (int j = 0; j < i; j++) {
		s += " ";
	}
	cout << s <<" / \\\n" << s << "| ";
	switch (type) {
	case Habitation:
		cout << "H";
		break;
	case Marche:
		cout << "M";
		break;
	case Caserne:
		cout << "C";
		break;
	case Temple:
		cout << "T";
		break;
	case Jardin:
		cout << "J";
		break;
	case Place:
		cout << getEtoiles();
		break;
	}
	cout << " | \n" << s << " \\ / ";
}