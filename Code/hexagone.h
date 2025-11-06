
#include <iostream>
#include <string>
#include <cstdio>
#ifndef HEXAGONE_H
#define HEXAGONE_H
using namespace std;

class HexaException {
	string info;
public:
	HexaException(const string& s) :info(s){}
	const string& getInfo() const { return info; }
};


typedef enum { Habitation, Marche, Caserne, Temple, Jardin, Place } Type;


class Hexagone {
private:
	friend class Tuiles;
	int niveau, etoiles;
	Type type;
	bool recouvert;
	void recouvrir(bool r);
	Hexagone& operator=(const Hexagone& h) = default;

public:
	Hexagone(const Hexagone& h) = default;
	Type getType() const { return type; }
	int getNiveau() const { return niveau; }
	bool est_recouvert() const { return recouvert; }
	int getEtoiles() const { return etoiles; }
	Hexagone() = default;
	Hexagone(Type t, int n = 1, bool r = false, int e = 0);
	~Hexagone() {}

    char affiche();
};

#endif // HEXAGONE_H