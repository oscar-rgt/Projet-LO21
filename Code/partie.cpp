#include "partie.h"

Partie::Partie(int id_, Mode m)
    : id(id_), mode(m), nbJoueurs(0)
{
    joueurs.fill(nullptr);
}

// Accesseurs
int Partie::getId() const { return id; }
Partie::Mode Partie::getMode() const { return mode; }
int Partie::getNbJoueurs() const { return nbJoueurs; }
Joueur* Partie::getJoueur(int index) const {
    if(index >= 0 && index < nbJoueurs) return joueurs[index];
    return nullptr;
}

bool Partie::ajouterJoueur(Joueur* j) {
    if(nbJoueurs < 4) {
        joueurs[nbJoueurs++] = j;
        return true;
    } else {
        std::cout << "Partie complète, impossible d'ajouter " << j->getNom() << std::endl;
        return false;
    }
}

// bucle de tours
void Partie::jouerTours(int maxTours) {
    for(int t = 0; t < maxTours; ++t) {
        std::cout << "=== Tour " << t+1 << " ===" << std::endl;
        for(int i = 0; i < nbJoueurs; ++i) {
            Joueur* j = joueurs[i];
            std::cout << "Tour de " << j->getNom() << std::endl;
        }
    }
}
