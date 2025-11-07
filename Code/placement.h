#pragma once
#include "tuiles.h"
#include "hexagone.h"

class Plateau {
private :
	Tuile** tab; //Tuiles placées sur le plateau dans l'ordre de placement
	Placement* coordonnees[3]; 
public :
	const Tuile** getTab() const { return tab; }
	void placer(Tuile*);
	Plateau();
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