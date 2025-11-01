#include <iostream>
#include <cstring>
#include "hexagone.h"


Hexagone::Hexagone(Type t, int n = 1, bool r = false, int e = 0) : type(t), niveau(n), recouvert(r) {
	if (t == Place) {
		etoiles = e;
	}
}

void Hexagone::recouvrir(bool r) {
	if (est_recouvert()) {
		throw HexaException("Hexagone deja recouvert");-
	}
	recouvert = r;
}