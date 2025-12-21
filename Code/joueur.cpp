
#include "joueur.h"
#include "cite.h"
#include "score.h"

using namespace std;

Joueur::Joueur(const string& n, unsigned int p): nom(n), pierres(p) {
    cite = new Cite();
    score = new Score(this);
    cite->placerTuileDepart();
}


void Joueur::ajouterPierres(unsigned int n) {
    pierres += n;
}

bool Joueur::utiliserPierres(unsigned int n) {
    if (pierres >= n) {
        pierres -= n;
        return true;
    }
    return false;
}


Joueur::~Joueur() {
    delete cite;
    delete score;
}
