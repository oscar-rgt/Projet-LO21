#include <iostream>
#include "placement.h"
#include "tuiles.h"

void Plateau::placer(Tuile* t, Coord c){
	if (!estLibre(c)||!estLibre(c.sudEst())||!estLibre(c.sudOuest())) { return; }
	carte[c] = t->getHexagone(0);
	carte[c.sudEst()] = t->getHexagone(1);
	carte[c.sudOuest()] = t->getHexagone(2);
}