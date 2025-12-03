#include <iostream>
#include <string>
#include <cstdio>
#ifndef HEXAGONE_H
#define HEXAGONE_H
using namespace std;

class Tuile;

class HexaException {
	string info;
public:
	HexaException(const string& s) :info(s) {}
	const string& getInfo() const { return info; }
};


typedef enum { Habitation, Marche, Caserne, Temple, Jardin, Carriere, Place } Type;


class Hexagone {
private:
	friend class Tuile;
	friend class TuileDepart;
	unsigned int niveau, etoiles;
	Tuile* tuile;
	Type type;
	bool place;
	Hexagone(Type t, unsigned int n = 0, Tuile* tu = nullptr, unsigned int e = 0, bool p = false) : type(t), niveau(n), tuile(tu), etoiles(e), place(p) {}
	~Hexagone() {}
	Hexagone() = default;
	Hexagone(const Hexagone& h) = default;
	Hexagone& operator=(const Hexagone& h) = default;
public:
	unsigned int getNiveau() const { return niveau; }
	Tuile* getTuile() const { return tuile; }
	Type getType() const { return type; }
	unsigned int getEtoiles() const { return etoiles; }
	string affiche();
};

#endif // HEXAGONE_H
