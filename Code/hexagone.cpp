#include <iostream>
#include <cstring>
#include "hexagone.h"
#include "tuiles.h"

using namespace std;


const string Hexagone::affiche() const{
	// Habitation, Marche, Caserne, Temple, Jardin, Place
	string niv = to_string(getNiveau()); //on récupère le niveau sous forme de string
	string res; //string où on va stocker le résultat
	if (place) {
		res += to_string(getEtoiles()); //si c'est une place, on ajoute le nb d'étoiles au début
	}
	else {
		res += " "; //sinon rien
	}
	switch (type) {
	//en fonction du type de quartier, on ajoute la lettre correspondante + le niveau
	case Habitation:
		res += "H" + niv; 
		break;
	case Marche:
		res += "M" + niv;
		break;
	case Caserne:
		res += "C" + niv;
		break;
	case Temple:
		res += "T" + niv;
		break;
	case Jardin:
		res += "J" + niv;
		break;
	case Carriere:
		res += "X" + niv;
		break;
	}
	return res;
}