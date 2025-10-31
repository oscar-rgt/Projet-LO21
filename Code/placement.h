#pragma once
#include "tuiles.h"

class Plateau {
private :
	Tuile** tab; //Tuiles placées sur le plateau dans l'ordre de placement
	int* voisins[3][4]; //voisins[t][h] = voisins de l'hexagone h de la tuile t
	int* etage; //etage de la tuile t;
public :
	const Tuile** getTab() const { return tab; }
	const int* getVoisins(int t, int h) const { return voisins[t][h]; }
	void placer(Tuile*);
	Plateau();
};