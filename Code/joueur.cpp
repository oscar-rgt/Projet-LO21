#include "joueur.h"

// Constructeur
Joueur::Joueur(const std::string& n)
    : nom(n), points(0), pierres(0) {}

// === Accesseurs ===
const std::string& Joueur::getNom() const {
    return nom;
}

int Joueur::getPoints() const {
    return points;
}

int Joueur::getPierres() const {
    return pierres;
}

// === Mutateurs / Actions ===
void Joueur::setPoints(int p) {
    points = p;
}

void Joueur::ajouterPoints(int delta) {
    points += delta;
    if (points < 0) points = 0; // sécurité
}

void Joueur::ajouterPierre(int n) {
    pierres += n;
    if (pierres < 0) pierres = 0;
}

bool Joueur::utiliserPierre(int n) {
    if (pierres >= n) {
        pierres -= n;
        return true;
    }
    return false;
}
