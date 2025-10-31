#include <iostream>
#include <cstring>


typedef enum { Habitation, Marche, Caserne, Temple, Jardin, Place } Type;


class Hexagone {
private:
	int niveau, etoiles;
	Type type;
	bool recouvert;

	Hexagone(Type t, int e = 0, int n = 1, bool r = false);
	~Hexagone() {}
	Hexagone(const Hexagone& h) = delete;
	Hexagone& operator=(const Hexagone&) = delete;
	

	Type getType() const { return type; }
	int getEtoiles() const { return etoiles; }
	int getNiveau() const { return niveau; }
};
