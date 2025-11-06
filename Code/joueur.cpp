#include "joueur.h"

Joueur::Joueur(const std::string& n)
    : nom(n), points(0), pierres(0) {}

const std::string& Joueur::getNom() const {
    return nom;
}

int Joueur::getPoints() const {
    return points;
}

int Joueur::getPierres() const {
    return pierres;
}

void Joueur::setPoints(int p) {
    points = p;
}

void Joueur::ajouterPoints(int delta) {
    points += delta;
    if (points < 0) points = 0;
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

void Joueur::ajouterPierres(int nbRecouverts) {
    if (nbRecouverts > 0) {
        pierres += nbRecouverts;
    }
}
