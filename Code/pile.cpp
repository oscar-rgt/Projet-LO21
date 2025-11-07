#include "pile.h"
#include <iostream>

Pile::Pile(int id_, int nb)
    : id(id_), nbTuiles(nb)
{
}

bool Pile::retirerTuile() {
    if(nbTuiles > 0) {
        nbTuiles--;
        return true;
    } else {
        std::cout << "La pile est vide !" << std::endl;
        return false;
    }
}
