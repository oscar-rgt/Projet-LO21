#include "ia.h"
#include "tuiles.h"
#include "hexagone.h"
#include "partie.h"
#include<map>
using namespace std;


IA::IA(const string& nom, int diff) : Joueur(nom), difficulte(diff), tuileDepart(new TuileDepart) {
	ajouterTuile(tuileDepart);
}


IA::~IA() {
    tuilesAcquises.clear();
    delete tuileDepart;
}

void IA::ajouterTuile(Tuile* t) {
    tuilesAcquises.push_back(t);
}


int IA::choisirTuile(const Chantier& chantier) {
    //Rappel règles :
    //L’Illustre Architecte cherche toujours à acquérir la moins chère des tuiles Cité comportant une Place.
    //S’il n’a pas assez de Pierres pour l’acquérir ou si aucune tuile Cité ne comporte de Place,
    //l’Illustre Architecte prend la 1ère tuile Cité du Chantier.

    int meilleurIndex = -1;
    int currentIndex = 0;

    for (auto it = chantier.begin(); it != chantier.end(); ++it) {
        Tuile* t = *it;
        int coutTuile = t->getPrix();

        if (t->contientPlace()) {
            //C'est une tuile avec Place
            //Comme on parcourt par index croissant (donc prix croissant),
            //la première qu'on trouve est forcément la moins chère.
            if (getPierres() >= coutTuile) {
                meilleurIndex = currentIndex;
                break;
            }
        }
        currentIndex++;
    }

    if (meilleurIndex != -1) return meilleurIndex;
    return 0;
}


