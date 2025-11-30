#pragma once

#include <vector>
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
protected: // Changed to protected for inheritance
	friend class Pile;
	unsigned int id;
	bool inversion;
	string design;
	vector<Hexagone*> hexagones; 
	unsigned int prix; 
	Tuile(const Tuile& t);
	Tuile& operator=(const Tuile& t);
	Tuile() = default;
	Tuile(unsigned int i, unsigned int p = 0);
	virtual ~Tuile();
public:
	unsigned int getId() const { return id; }
	bool getInversion() const { return inversion; }
	void inverser() { inversion = !inversion; }
	Hexagone* getHexagone(int i) { return hexagones[i]; }
    size_t getNbHexagones() const { return hexagones.size(); } 
	unsigned int getPrix() const { return prix; }
	void tourner();
	void setPrix(unsigned int p);
	string& getDesign() { return design; }
};

class TuileDepart : public Tuile {
private:
	friend class Cite;
    TuileDepart();
};


int randomIndexAkropolis();
int randomPlaceValue();
int randomStarValue();
