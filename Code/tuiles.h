#include <iostream>
#include <cstring>
#include "Hexagone.h"

using namespace std;

enum Orientation {N, SE, SO};

class Tuile{
private :
	friend class Hexagone;
	int id;
	Orientation orientation;
	Hexagone hexagones[3];

	
	Tuile(const Tuile& t) = delete;
	Tuile& operator=(const Tuile& t) = delete;

	int getId() const { return id; }
	const Hexagone& getHexagone(int i) const { return hexagones[i]; }
	Orientation getOrientation() const { return orientation; }

public:
	void affiche();
	Tuile(int i, const Hexagone& h1, const Hexagone& h2, const Hexagone& h3);
	~Tuile() {}
};