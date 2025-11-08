#pragma once
#include "tuiles.h"
#include "hexagone.h"
#include <unordered_map>

using namespace std;

struct Coord { //Tuple de coordonnées 
	int x, y, z;
	Coord sudOuest() { return { x,y + 1,z }; }
	Coord sudEst() { return { x+1 ,y + 1,z }; }// A changer en odd-q + symétrie axiale "haut/bas"
	bool operator==(const Coord& other) const noexcept { return x == other.x && y == other.y && z == other.z; }
};

struct CoordHash {
	size_t operator()(const Coord& c) const noexcept {
		size_t hx = hash<int>()(c.x);
		size_t hy = hash<int>()(c.y);
		size_t hz = hash<int>()(c.z);
		// combine les trois hash (mélange simple et rapide)
		return hx ^ (hy << 1) ^ (hz << 2);
	}
};

class Cite {
private :
	unordered_map<Coord, Hexagone*, CoordHash> carte; // Espace 3D de références d'hexagones 
	//  carte[{0, 0, 0}] = tuile0;
	//vector<Coord> voisinsTuile(Coord c); // Bloque la compilation : pk un vector ? fct à définir

public :
	void placer(Tuile* t, Coord c);
	bool estLibre(Coord c) { return (carte.find(c) == carte.end()); }
};
