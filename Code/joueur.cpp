
#include "joueur.h"
#include "cite.h"
#include "score.h"

using namespace std;

Joueur::Joueur(const string& n)
    : nom(n), pierres(2) {
    cite = new Cite();
    score = new Score(this);
    cite->placerTuileDepart();
}


void Joueur::ajouterPierres(unsigned int n) {
    pierres += n;
    if (pierres < 0) pierres = 0;
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
