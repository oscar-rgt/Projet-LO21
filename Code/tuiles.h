#pragma once

#include <iostream>
#include <cstring>
#include "hexagone.h"
using namespace std;

class TuileException {
	string info;
public:
	TuileException(const string& s) :info(s) {}
	const string& getInfo() const { return info; }
};

class Tuile{
private:
	friend class Pile;
	unsigned int id;
	unsigned int niveau;
	bool inversion;
	Hexagone hexagones[3]; // faire une méthode qui tourne la tuile pour que l'hexagone au nord soit tjrs le 0, on tourne dans le sens des aiguilles d'une montre (SE = 1, SO =2)
	unsigned int prix; // faire une méthode inverser qui symétrise la tuile
	Tuile(const Tuile& t) = default;
	Tuile& operator=(const Tuile& t) = default;
	Tuile(unsigned int i, unsigned int p = 0, unsigned int n = 1);
	Tuile() = default;
	~Tuile() {}
public:
	unsigned int getId() const { return id; }
	bool getInversion() const { return inversion; }
	void inverser() { inversion = !inversion; }
	Hexagone& getHexagone(int i) { return hexagones[i]; }
	unsigned int getPrix() const { return prix; }
	void setPrix(unsigned int p);
};


int randomIndexAkropolis();
int randomPlaceValue();
int randomStarValue();
