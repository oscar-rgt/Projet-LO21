
#include "joueur.h"
#include "cite.h"

using namespace std;

Joueur::Joueur(const string& n)
    : nom(n), points(0), pierres(2) {
    cite = new Cite();
    cite->placerTuileDepart();
}

const string& Joueur::getNom() const {
    return nom;
}

int Joueur::getPoints() const {
    return points;
}

int Joueur::getPierres() const {
    return pierres;
}

void Joueur::setPoints(int p) {
    points = (p >= 0) ? p : 0;
}

void Joueur::ajouterPoints(int delta) {
    points += delta;
    if (points < 0) points = 0;
}

void Joueur::ajouterPierres(int n) {
    pierres += n;
    if (pierres < 0) pierres = 0;
}

bool Joueur::utiliserPierres(int n) {
    if (pierres >= n) {
        pierres -= n;
        return true;
    }
    return false;
}

void Joueur::ajouterPierresRecouvrement(int nbRecouverts) {
    if (nbRecouverts > 0) {
        ajouterPierres(nbRecouverts);
    }
}

Joueur::~Joueur() {
    delete cite;
}


