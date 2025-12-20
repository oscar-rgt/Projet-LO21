#pragma once

#include <vector>
#include <cstring>
#include "hexagone.h"

using namespace std;

class Pile; // Forward declaration pour éviter les inclusions circulaires

// Classe représentant une Tuile (groupe de 3 hexagones)
class Tuile {
protected:
	// La classe Pile a besoin d'accéder aux constructeurs privés/protégés
	friend class Pile;

	unsigned int id; // Identifiant unique de la tuile
	bool inversion; // État : Est-ce que la tuile est retournée
    unsigned int prix; // Coût en pierres
	string design; // Représentation ASCII

	// Conteneur des hexagones.
	vector<Hexagone*> hexagones;

	Tuile(const Tuile& t);
	Tuile& operator=(const Tuile& t);
	Tuile() = default;
	Tuile(unsigned int i, unsigned int p = 0);

	// Destructeur virtuel pour TuileDepart
	virtual ~Tuile();
	
public:
	// Accesseurs
	unsigned int getId() const { return id; }
	bool getInversion() const { return inversion; }
	Hexagone* getHexagone(int i) const { return hexagones[i]; } 
    int getNbHexagones() const { return static_cast<int>(hexagones.size()); }
	unsigned int getPrix() const { return prix; }
	string& getDesign();

	void inverser() { inversion = !inversion; }
	void tourner();
	void setPrix(unsigned int p); //utile pour le chantier qui set le prix des tuiles créees dans la pile

	// Méthode pour charger une sauvegarde (restaure l'état interne)
	void reconstruireHexagone(int index, int typeInt, int etoiles);

	// Méthode statique pour créer une tuile sans passer par une Pile
	// (Utile pour le chargement de partie)
	static Tuile* fabriquer(unsigned int id, unsigned int prix) {return new Tuile(id, prix);}
};

// Tuile spécifique : tuile avec 4 hexagones 
class TuileDepart : public Tuile {
private:
	friend class IA;
	friend class Cite;
	TuileDepart();
};

int randomIndexAkropolis();
int randomPlaceValue();
int randomStarValue();