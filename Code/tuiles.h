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
	Hexagone hexagones[3]; // faire une méthode qui tourne la tuile pour que l'hexagone au nord soit tjrs le 0, on tourne dans le sens des aiguilles d'une montre (SE = 1, SO =2)
	unsigned int prix;
	Tuile(const Tuile& t) = delete;
	Tuile& operator=(const Tuile& t) = delete;
public:
	unsigned int getId() const { return id; }
	Orientation getOrientation() const { return orientation; }
	Hexagone& getHexagone(int i) { return hexagones[i]; }
	unsigned int getPrix() const { return prix; }
	Tuile(unsigned int i, Hexagone& h1, Hexagone& h2, Hexagone& h3, unsigned int p) : id(i), orientation(N), hexagones{ h1, h2, h3 }, prix(p) {}
	~Tuile() {}
};


