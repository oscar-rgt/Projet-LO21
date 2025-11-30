#include "pile.h"
#include <iostream>
#include <string>
#include<vector>

Pile::Pile(unsigned int id_, unsigned int nb): id(id_)
{
    tuiles.reserve(nb);
    for (unsigned int i = 0; i < nb; i++) {
        tuiles.push_back(new Tuile(id * 10 + i, i + 1));
    }
}

Pile::~Pile() {
    for (size_t i = 0; i < tuiles.size(); i++) {
        delete tuiles[i];
    }
    tuiles.clear();
}


Tuile* Pile::getTuile(unsigned int id_) {
    for (size_t i = 0; i < tuiles.size(); i++) {
        if (tuiles[i]->getId() == id_) {
            return tuiles[i];
        }
    }
    throw PileException("La tuile n'existe pas");
}

void Pile::decalagePrix() {
    if (!tuiles.empty()) {
        for (size_t i = 0; i < tuiles.size(); i++) {
            if (tuiles[i]->getPrix() != 0) {
                tuiles[i]->setPrix(tuiles[i]->getPrix() - 1);
            }
        }
    }
}


