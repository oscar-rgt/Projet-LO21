#include <iostream>
#include <cstring>
#include "Hexagone.h"

typedef enum{1 2 3} Orientation;

class Tuile{
private :
	int id;
	Orientation orientation;
	Hexagone hexagones[3];

	Tuile(int i, const Hexagone& h1, const Hexagone& h2, const Hexagone& h3) : id(i), hexagones({ h1, h2, h3 }), orientation(1) {}
	~Tuile(){}
	Tuile(const Tuile& t) = delete;
	Tuile& operator=(const Tuile& t) = delete;

	int getId() const { return id; }
	const Hexagone& getHexagone(int i) const { return hexagones[i]; }
	Orientation getOrientation() const { return Orientation; }
};