#include "pile.h"
#include <iostream>

Pile::Pile(unsigned int id_): id(id_), nbTuiles(3)
{
    for (unsigned int i = 0; i < 3; i++) {
        tuiles[i] = Tuile(id * 10 + i, i + 1);
    }
}

void Pile::retirerTuile(unsigned int id_) {
    if(nbTuiles > 0) {
        for (unsigned int i = 0; i < nbTuiles; i++) {
            if (tuiles[i].getId() == id_) {
                tuiles[i] = NULL;
                nbTuiles--;
            }
        }
    } 
    else {
        throw PileException("La pile est vide !");
    }
}


Tuile Pile::getTuile(unsigned int id_) const {
    for (unsigned int i = 0; i < nbTuiles; i++) {
        if (tuiles[i].getId() == id_) {
            return tuiles[i];
        }
    }
    throw PileException("La tuile n'existe pas");
}

void Pile::decalagePrix() {
    if (nbTuiles != 0) {
        for (unsigned int i = 0; i < nbTuiles; i++) {
            if (tuiles[i].getPrix() != 0) {
                tuiles[i].setPrix(tuiles[i].getPrix() - 1);
            }
        }
    }
}