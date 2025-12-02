#include "chantier.h"


Chantier::Chantier() {}

Chantier::~Chantier() {
    vider();
}

void Chantier::vider() {
    tuiles.clear();
}

void Chantier::ajouterPile(Pile& p) {
    for (int j = 0; j < p.getNbTuiles(); j++) {
        try {
                Tuile* t = p.getTuile(p.getId()*10+j);
                tuiles.push_back(t);
        } catch (const PileException& e) {
            cout << "erreur ; " << e.getInfo();
            break;
        }
    }
}


Tuile* Chantier::getTuile(int index) const {
    if (index < 0 || index >= (int)tuiles.size()) return nullptr;
    return *(tuiles.begin() + index);
}

void Chantier::retirerTuile(int index) {
    if (index < 0 || index >= (int)tuiles.size()) return;
    // Le pointeur est retiré du vecteur
    tuiles.erase(tuiles.begin() + index);
}

size_t Chantier::getNbTuiles() const {
    return tuiles.size();
}

bool Chantier::estVide() const {
    return tuiles.empty();
}

void Chantier::ajouterTuileSpecifique(Tuile* t) {
    if (t) {
        tuiles.push_back(t);
    }
}
