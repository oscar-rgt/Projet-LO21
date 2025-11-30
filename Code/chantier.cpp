#include "chantier.h"
#include"pile.h"

Chantier::Chantier() {}

Chantier::~Chantier() {
    vider();
}

void Chantier::vider() {
    tuiles.clear();
    pierres.clear();
}

void Chantier::ajouterPile(Pile& p) {
    while (!p.estVide()) {
        try {
            for(int i=0; i<p.getNbTuiles(); i++) {
                Tuile* t = p.getTuile(p.getId()*10+i);
                tuiles.push_back(t);
                pierres.push_back(0); // Initialiser avec 0 pierre
            }
        } catch (const PileException& e) {
            cout << "erreur ; " << e.getInfo();
            break;
        }
    }
}


Tuile* Chantier::getTuile(int index) const {
    if (index < 0 || index >= tuiles.size()) return nullptr;
    return tuiles[index];
}

void Chantier::retirerTuile(int index) {
    if (index < 0 || index >= tuiles.size()) return;
    // Le pointeur est retiré du vecteur
    tuiles.erase(tuiles.begin() + index);
    pierres.erase(pierres.begin() + index);
}

size_t Chantier::getNbTuiles() const {
    return tuiles.size();
}

bool Chantier::estVide() const {
    return tuiles.empty();
}

int Chantier::getPierres(int index) const {
    if (index < 0 || index >= pierres.size()) return 0;
    return pierres[index];
}

void Chantier::ajouterPierre(int index) {
    if (index >= 0 && index < pierres.size()) {
        pierres[index]++;
    }
}

int Chantier::ramasserPierres(int index) {
    if (index < 0 || index >= pierres.size()) return 0;
    int p = pierres[index];
    return p;
}
