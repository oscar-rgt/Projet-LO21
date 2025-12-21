#include "pile.h"
#include <vector>

Pile::Pile(unsigned int id_, unsigned int nb) : id(id_) {
    tuiles.reserve(nb);
    for (unsigned int i = 0; i < nb; i++) {
        Tuile* t = new Tuile(id_ * 10 + i, i + 1); // L'ID de la tuile est dérivé de l'ID de la pile (ex: Pile 1 -> Tuiles 10, 11, 12...)
		tuiles.push_back(t); //ajout de la tuile à la pile
    }
}

Pile::~Pile() {
    for (auto it = tuiles.begin(); it != tuiles.end(); ++it) {
        delete* it; // Supprime l'objet Tuile en mémoire
    }
    // Vide le vecteur
    tuiles.clear();
}
