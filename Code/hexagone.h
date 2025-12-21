#ifndef HEXAGONE_H
#define HEXAGONE_H
#include <iostream>
#include <string>
#include <cstdio>

using namespace std;

class Tuile;

typedef enum { Habitation, Marche, Caserne, Temple, Jardin, Carriere} TypeQuartier;


class Hexagone {
private:
	friend class Tuile;
	friend class TuileDepart;
    TypeQuartier type;
    unsigned int niveau;
	Tuile* tuile;
    unsigned int etoiles;
	bool place;
	Hexagone(TypeQuartier t, unsigned int n = 0, Tuile* tu = nullptr, unsigned int e = 0, bool p = false) : type(t), niveau(n), tuile(tu), etoiles(e), place(p) {}
	~Hexagone() {}
	Hexagone() = default;
	Hexagone(const Hexagone& h) = default;
	Hexagone& operator=(const Hexagone& h) = default;
public:
	// Accesseurs
	unsigned int getNiveau() const { return niveau; }
	Tuile* getTuile() const { return tuile; }
	TypeQuartier getType() const { return type; }
	unsigned int getEtoiles() const { return etoiles; }
	bool estPlace() const { return place; }

	// Méthode d'affichage et setteur
	const string affiche() const;
	void setNiveau(unsigned int n) { niveau = n; }
    
};

#endif 
