#include <iostream>
#include <cstring>
#include "tuiles.h"
#include "hexagone.h"

int main() {
	Hexagone h1(Temple);
	Hexagone h2(Jardin);
	Hexagone h3(Habitation);
	Tuile t1(0, h1, h2, h3);

	t1.affiche();

	return 0;
}