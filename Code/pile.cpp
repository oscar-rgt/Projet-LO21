#include "pile.h"
#include <iostream>
#include <string>
#include<vector>

Pile::Pile(unsigned int id_, unsigned int nb): id(id_)
{
    tuiles.reserve(nb);
    for (unsigned int i = 0; i < nb; i++) {
        tuiles.emplace_back(id * 10 + i, i + 1);
    }
}



Tuile* Pile::getTuile(unsigned int id_) {
    for (unsigned int i = 0; i < tuiles.size(); i++) {
        if (tuiles[i].getId() == id_) {
            return &tuiles[i];
        }
    }
    throw PileException("La tuile n'existe pas");
}

void Pile::decalagePrix() {
    if (!tuiles.empty()) {
        for (unsigned int i = 0; i < tuiles.size(); i++) {
            if (tuiles[i].getPrix() != 0) {
                tuiles[i].setPrix(tuiles[i].getPrix() - 1);
            }
        }
    }
}


void Pile::affiche() {
    vector<string> lignes(9, "");
    const int hauteur = 9;
    const int largeur_reelle = 20;
    const int largeur_visuelle = 19;

    for (unsigned int i = 0; i < tuiles.size(); i++) {
        string designTuile = tuiles[i].getDesign();
        for (int j = 0; j < hauteur; j++) {
            string segment = designTuile.substr(j * largeur_reelle, largeur_visuelle);
            lignes[j] += segment;
            lignes[j] += "         ";
        }
    }
    for (int j = 0; j < hauteur; j++) {
        cout << lignes[j] << endl;
    }
}

