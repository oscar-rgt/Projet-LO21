#pragma once

#include <iostream>
#include <cstring>
#include "hexagone.h"
using namespace std;

class Pile;

class TuileException {
	string info;
public:
	TuileException(const string& s) :info(s) {}
	const string& getInfo() const { return info; }
};

class Tuile{
private:
	friend class Pile;
	friend class Partie;
	unsigned int id;
	unsigned int niveau;
	bool inversion;
	string design;
	Hexagone hexagones[3]; // faire une m�thode qui tourne la tuile pour que l'hexagone au nord soit tjrs le 0, on tourne dans le sens des aiguilles d'une montre (SE = 1, SO =2)
	unsigned int prix; // faire une m�thode inverser qui sym�trise la tuile
	Tuile(const Tuile& t);
	Tuile& operator=(const Tuile& t);
	Tuile() = default;
	Tuile(unsigned int i, unsigned int p = 0, unsigned int n = 1);
	~Tuile() {}
public:
	unsigned int getId() const { return id; }
	bool getInversion() const { return inversion; }
	void inverser() { inversion = !inversion; }
	Hexagone& getHexagone(int i) { return hexagones[i]; }
	unsigned int getPrix() const { return prix; }
	void tourner();
	void setPrix(unsigned int p);
	string& getDesign() { return design; }
};


int randomIndexAkropolis();
int randomPlaceValue();
int randomStarValue();
