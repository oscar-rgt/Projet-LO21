#include "ia.h"
#include "tuiles.h"
#include "hexagone.h"
#include "partie.h"
#include "ia.h"
#include <iostream>

using namespace std;



IA::~IA() {
    tuilesAcquises.clear();
}

void IA::ajouterTuile(Tuile* t) {
    tuilesAcquises.push_back(t);
}

bool IA::tuileContientPlace(Tuile* t) {
    if (!t) return false;
    for (size_t i = 0; i < t->getNbHexagones(); ++i) {
        Hexagone* h = t->getHexagone(i);
        if (h->getType() == Place) {
            return true;
        }
    }
    return false;
}

int IA::choisirTuile(const Chantier& chantier) {
    // L’Illustre Architecte cherche toujours à acquérir la moins chère des tuiles Cité comportant une Place.
    // S’il n’a pas assez de Pierres pour l’acquérir ou si aucune tuile Cité ne comporte de Place,
    // l’Illustre Architecte prend la 1ère tuile Cité du Chantier.

    int meilleurIndex = -1;
    
    for (int i = 0; i < chantier.getNbTuiles(); ++i) {
        Tuile* t = chantier.getTuile(i);
        int coutTuile = t->getPrix(); 

        if (tuileContientPlace(t)) {
            // C'est une tuile avec Place
            // Comme on parcourt par index croissant (donc prix croissant),
            // la première qu'on trouve est forcément la moins chère.
            
            if (getPierres() >= coutTuile) {
                meilleurIndex = i;
                break; // On a trouvé la moins chère abordable
            }
        }
    }

    if (meilleurIndex != -1) {
        return meilleurIndex;
    } else {
        // Pas de tuile avec Place ou pas assez de pierres
        return 0; // Prend la 1ère tuile
    }
}


//BONNE VERSION SCORE IA
int IA::calculerScoreIA() const {

    //niveau 1 : quartiers : 1pt / carriere : 0pt
    //niveau 2 : quartiers : 1pt / carriere : 2pt
    //niveau 3 : quartiers : *2 (points doublés) / carriere : 0pt    
    int totalScore = 0;

    int multiplicateurNiveau = (difficulte == 3) ? 2 : 1;

    for (Tuile* t : tuilesAcquises) {
        for (size_t i = 0; i < t->getNbHexagones(); i++) {
            Hexagone* h = t->getHexagone(i);
            Type type = h->getType();

            if (type == Carriere) {
                if (difficulte == 2) {
                    totalScore += 2; // bonus Carriere niveau 2
                }
                continue;
            }

            int pts = 0;
            switch (type) {
                case Habitation:
                    pts = 1; // on considère le placement correct
                    break;
                case Marche:
                    pts = 1;
                    break;
                case Caserne:
                    pts = 1;
                    break;
                case Temple:
                    pts = 1;
                    break;
                case Jardin:
                    pts = 1;
                    break;
                default:
                    pts = 0;
            }

            totalScore += pts * multiplicateurNiveau;
        }
    }

    return totalScore;
}

