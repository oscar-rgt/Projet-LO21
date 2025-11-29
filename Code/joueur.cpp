#include "joueur.h"

using namespace std;

Joueur::Joueur(const string& n)
    : nom(n), pierres(0), cite(nullptr)
{
    score = new Score(this);
}

Joueur::~Joueur() {
    delete score;
}


const string& Joueur::getNom() const {
    return nom;
}

Cite* Joueur::getCite() const { return cite; }
void Joueur::setCite(Cite* c) { cite = c; }

Score* Joueur::getScore() const { return score; } //lire les points

void Joueur::calculerScore() { //mettre à jour le score
    if (score)
        score->calculerScore();
}

int Joueur::getPierres() const { return pierres; }

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
    if (nbRecouverts > 0)
        ajouterPierres(nbRecouverts);
}
