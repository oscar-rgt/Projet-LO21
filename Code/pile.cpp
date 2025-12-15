#include "pile.h"
#include <iostream>
#include <string>
#include <vector>

Pile::Pile(unsigned int id_, unsigned int nb) : id(id_)
{
    tuiles.reserve(nb);
    for (unsigned int i = 0; i < nb; i++) {
        tuiles.push_back(new Tuile(id * 10 + i, i + 1));
    }
}

Pile::~Pile() {
    for (auto it = tuiles.begin(); it != tuiles.end(); ++it) {
        delete* it;
    }
    tuiles.clear();
}
