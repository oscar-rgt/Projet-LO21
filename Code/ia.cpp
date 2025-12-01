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

   
int IA::calculerScoreIA(const std::array<bool, 5>& variantesActives) const {

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
                if (difficulte == 2) totalScore += 2;
                continue;
            }

            int pts = 1; 

            //avec les variantes
            switch (type) {
                case Habitation:
                    if (variantesActives[0]) { 
                        pts *= 2; 
                    }
                    break;
                case Marche:
                    if (variantesActives[1]) { 
                        pts *= 2; 
                    }
                    break;
                case Caserne:
                    if (variantesActives[2]) { 
                        pts *= 2;
                    }
                    break;
                case Temple:
                    if (variantesActives[3]) { 
                        pts *= 2; 
                    }
                    break;
                case Jardin:
                    if (variantesActives[4]) { 
                        pts *= 2; 
                    }
                    break;
                default:
                    break;
            }

            totalScore += pts * multiplicateurNiveau;
        }
    }

    return totalScore;
}

