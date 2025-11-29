#include "pile.h"
#include <iostream>

Pile::Pile(unsigned int id_, unsigned int nb): id(id_), nbTuiles(nb)
{
    tuiles = new Tuile[nb];
    for (unsigned int i = 0; i < nb; i++) {
        tuiles[i] = Tuile(id * 10 + i, i + 1);
    }
}

Pile::~Pile() {
    delete[] tuiles; 
}

void Pile::retirerTuile(unsigned int id_) {
    if (nbTuiles == 0) {
        throw PileException("La pile est vide !");
    }
    unsigned int index = nbTuiles;
    for (unsigned int i = 0; i < nbTuiles; i++) {
        if (tuiles[i].getId() == id_) {
            index= i;
            break;
        }
    }
    nbTuiles--;
    if (nbTuiles== 0) {
        delete[] tuiles;
        tuiles = nullptr;
    }
    else {
        Tuile* nouv = new Tuile[nbTuiles];
        for (unsigned int i = 0; i < index; i++) {
            nouv[i] = tuiles[i];
        }
        for (unsigned int i = index; i < nbTuiles; ++i) {
            nouv[i] = tuiles[i];
        }
        delete[] tuiles;
        tuiles = nouv;
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