#pragma once

#include <iostream>
#include <cstring>
#include "hexagone.h"

using namespace std;

enum Orientation {N, SE, SO};

class Tuile{
private :
	unsigned int id;
	Orientation orientation;
	Hexagone hexagones[3];
	unsigned int prix;
	
	Tuile(const Tuile& t) = delete;
	Tuile& operator=(const Tuile& t) = delete;

	int getId() const { return id; }	
public:
	Orientation getOrientation() const { return orientation; }
	Hexagone& getHexagone(int i) { return hexagones[i]; }
	unsigned int getPrix() const { return prix; }
	void affiche();
	Tuile(unsigned int i, Hexagone& h1, Hexagone& h2, Hexagone& h3, unsigned int p) : id(i), orientation(N), hexagones{ h1, h2, h3 }, prix(p) {}
	~Tuile() {}
};


