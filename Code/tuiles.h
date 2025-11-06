#pragma once

#include <iostream>
#include <cstring>
#include "hexagone.h"

using namespace std;

enum Orientation {N, SE, SO};

class Tuile{
private :
	int id;
	Orientation orientation;
	Hexagone hexagones[3];

	
	Tuile(const Tuile& t) = delete;
	Tuile& operator=(const Tuile& t) = delete;

	int getId() const { return id; }	
public:
	Orientation getOrientation() const { return orientation; }
	Hexagone& getHexagone(int i) { return hexagones[i]; }
	void affiche();
	Tuile(int i, Hexagone& h1, Hexagone& h2, Hexagone& h3) : id(i), orientation(N), hexagones{ h1, h2, h3 } {}
	~Tuile() {}
};


