#include "chantier.h"
#include <algorithm> // Pour std::find

Chantier::Chantier() {}

Chantier::~Chantier() {
    vider();
}

void Chantier::vider() {
    tuiles.clear();
}

void Chantier::ajouterPile(Pile& p) {
    // Utilisation de l'itérateur de Pile 
    for (auto it = p.begin(); it != p.end(); ++it) {
        tuiles.push_back(*it);
    }
}



void Chantier::retirerTuile(Tuile* t) {
    auto it = std::find(tuiles.begin(), tuiles.end(), t);
    if (it != tuiles.end()) {
        tuiles.erase(it);
    }
}



bool Chantier::estVide() const {
    return tuiles.empty();
}

void Chantier::ajouterTuileSpecifique(Tuile* t) {
    if (t) {
        tuiles.push_back(t);
    }
}