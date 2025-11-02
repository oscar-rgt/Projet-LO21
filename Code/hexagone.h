
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
	Type getType() const { return type; }
	int getNiveau() const { return niveau; }
	bool est_recouvert() const { return recouvert; }
	void recouvrir(bool r);
	int getEtoiles() const { return etoiles; }

	

public:
	
	Hexagone(Type t, int n = 1, bool r = false, int e=0);
	Hexagone& operator=(const Hexagone&) = default;
	Hexagone(const Hexagone& h) = default;
	Hexagone() = default;
	~Hexagone() {}

    char affiche();
};

#endif // HEXAGONE_H