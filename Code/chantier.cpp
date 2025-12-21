#include "chantier.h"
#include <algorithm> // Pour std::find

using namespace std;

Chantier::Chantier() {}

Chantier::~Chantier() {
    vider();
}

void Chantier::ajouterPile(Pile& p) {
    for (auto it = p.begin(); it != p.end(); ++it) {
        tuiles.push_back(*it);
    }
}

void Chantier::retirerTuile(Tuile* t) {
    auto it = find(tuiles.begin(), tuiles.end(), t);
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