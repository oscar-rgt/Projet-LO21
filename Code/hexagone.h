#include <iostream>
#include <string>
#include <cstdio>
#ifndef HEXAGONE_H
#define HEXAGONE_H
using namespace std;

class Tuile;


typedef enum { Habitation, Marche, Caserne, Temple, Jardin, Carriere} TypeQuartier;


class Hexagone {
private:
	friend class Tuile;
	friend class TuileDepart;
	unsigned int niveau, etoiles;
	Tuile* tuile;
	TypeQuartier type;
	bool place;
	Hexagone(TypeQuartier t, unsigned int n = 0, Tuile* tu = nullptr, unsigned int e = 0, bool p = false) : type(t), niveau(n), tuile(tu), etoiles(e), place(p) {}
	~Hexagone() {}
	Hexagone() = default;
	Hexagone(const Hexagone& h) = default;
	Hexagone& operator=(const Hexagone& h) = default;
public:
	unsigned int getNiveau() const { return niveau; }
	Tuile* getTuile() const { return tuile; }
	TypeQuartier getType() const { return type; }
	unsigned int getEtoiles() const { return etoiles; }
	string affiche();
	void setNiveau(unsigned int n) { niveau = n; }
	const bool estPlace() const { return place; }
};

#endif // HEXAGONE_H
