#pragma once
#include "tuiles.h"
#include "hexagone.h"
#include <unordered_map>

struct Coord { //Tuple de coordonnées 
	int x, y, z;
	Coord sudEst() { return { x,y + 1,z }; }
	Coord sudOuest() { return { x+1 ,y + 1,z }; }
	bool operator==(const Coord& other) { return x == other.x && y == other.y && z == other.z; }
};

struct CoordHash {
	size_t operator()(const Coord& c) const noexcept {
		size_t hx = std::hash<int>()(c.x);
		size_t hy = std::hash<int>()(c.y);
		size_t hz = std::hash<int>()(c.z);
		// combine les trois hash (mélange simple et rapide)
		return hx ^ (hy << 1) ^ (hz << 2);
	}
};

class Plateau {
private :
	std::unordered_map<Coord, Hexagone&, CoordHash> carte; // Espace 3D de pointeurs vers des hexagones 
	//  carte[{0, 0, 0}] = tuile0;

public :
	void placer(Tuile* t, Coord c);
	bool estLibre(Coord c) { return (carte.find(c) == carte.end()) }
};

class Placement {
private:
	Hexagone& h;
	int colonne;
	int ligne;
	int niveau;
	Placement();
	friend class Plateau;
public:
	int getCol() { return colonne; }
	int getLi() { return ligne; }
	int getNiv() { return niveau; }
};