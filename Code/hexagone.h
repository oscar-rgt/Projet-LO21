
#include <iostream>
#include <string>
#include <cstdio>
#include"tuiles.h"
#ifndef HEXAGONE_H
#define HEXAGONE_H
using namespace std;

class HexaException {
	string info;
public:
	HexaException(const string& s) :info(s){}
	const string& getInfo() const { return info; }
};


typedef enum { Habitation, Marche, Caserne, Temple, Jardin, Place, Carriere} Type;


class Hexagone {
private:
	friend class Tuile;
	unsigned int niveau, etoiles;
	Tuile* t;
	Type type;
	bool recouvert;
	void recouvrir(bool r);
	Hexagone(Type t, unsigned int n = 1, Tuile* tuile = nullptr, unsigned int e = 0, bool r = false ): type(t), niveau(n), t(tuile), etoiles(e), recouvert(r)   {}
	~Hexagone() {}
	Hexagone() = default;
	Hexagone(const Hexagone& h) = default;
	Hexagone& operator=(const Hexagone& h) = default;
public:
	Type getType() const { return type; }
	int getNiveau() const { return niveau; }
	bool est_recouvert() const { return recouvert; }
	int getEtoiles() const { return etoiles; }
    char affiche();
};

#endif // HEXAGONE_H