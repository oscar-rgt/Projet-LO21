#include <iostream>
#include <cstring>
#include "tuiles.h"
#include "hexagone.h"

using namespace std;

Tuile::Tuile(int i, const Hexagone& h1, const Hexagone& h2, const Hexagone& h3) : id(i), orientation(N) {
	hexagones[0] = h1;
	hexagones[1] = h2;
	hexagones[2] = h3;
}

void Tuile::affiche() {
	hexagones[0].affiche(5);
	cout << "\n";
	hexagones[2].affiche(8);
	cout << "\r";
	hexagones[1].affiche(1);

}

void Quadrillage() {
	cout << "         _____         _____         _____\n";
	cout << "        /     \\       /     \\       /     \\\n";
	cout << "  _____/       \\_____/       \\_____/       \\_____\n";
	cout << " /     \\       /     \\       /     \\       /     \\\n";
	cout << "/       \\_____/       \\_____/       \\_____/       \\\n";
	cout << "\\       /     \\       /     \\       /     \\       /\n";
	cout << " \\_____/       \\_____/       \\_____/       \\_____/\n";
	cout << " /     \\       /     \\       /     \\       /     \\\n";
	cout << "/       \\_____/       \\_____/       \\_____/       \\\n";
	cout << "\\       /     \\       /     \\       /     \\       /\n";
	cout << " \\_____/       \\_____/       \\_____/       \\_____/\n";
	cout << " /     \\       /     \\       /     \\       /     \\\n";
	cout << "/       \\_____/       \\_____/       \\_____/       \\\n";
	cout << "\\       /     \\       /     \\       /     \\       /\n";
	cout << " \\_____/       \\_____/       \\_____/       \\_____/\n";
}



