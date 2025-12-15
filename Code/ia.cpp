#include "ia.h"
#include "tuiles.h"
#include "hexagone.h"
#include "partie.h"

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
        Hexagone* h = t->getHexagone((int)i);
        if (h->estPlace()) {
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
    int currentIndex = 0;

    for (auto it = chantier.begin(); it != chantier.end(); ++it) {
        Tuile* t = *it;
        int coutTuile = t->getPrix();

        if (tuileContientPlace(t)) {
            // C'est une tuile avec Place
            // Comme on parcourt par index croissant (donc prix croissant),
            // la première qu'on trouve est forcément la moins chère.
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


//BONNE VERSION SCORE IA
int IA::calculerScoreIA() const {

    //niveau 1 : quartiers : 1pt / carriere : 0pt
    //niveau 2 : quartiers : 1pt / carriere : 2pt
    //niveau 3 : quartiers : *2 (points doublés) / carriere : 0pt
    int totalScore = 0;
    
    int multiplicateurNiveau = (difficulte == 3) ? 2 : 1;

    // --- 1. SIMPLIFICATION DU PARCOURS ---
    for (auto it = begin(); it != end(); ++it) {
        Tuile* t = *it;

        for (size_t i = 0; i < t->getNbHexagones(); i++) {
            Hexagone* h = t->getHexagone(i);
            TypeQuartier type = h->getType();

            // --- 2. SIMPLIFICATION DU SCORE ---
            if (type == Carriere) {
                // Seul le niveau 2 (Métagénès) gagne des points avec les carrières
                if (difficulte == 2) {
                    totalScore += 2; 
                }
            } 
            else {
                // Tous les autres quartiers (Habitation, Temple, etc.) rapportent 1 point de base
                // On applique directement le multiplicateur de difficulté
                totalScore += 1 * multiplicateurNiveau;
            }
        }
    }

    return totalScore;
}
