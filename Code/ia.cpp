#include "ia.h"
#include "tuiles.h"
#include "hexagone.h"
#include <iostream>

using namespace std;



IA::~IA() {
    /*for (auto t : tuilesAcquises) {
        delete t;
    }*/ //C'est partie qui doit supprimer les tuiles nn? IA n'y accède qu'en lecture
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

int IA::calculerScoreIA() const {
    int score = 0;
    
    // Niveaux de difficulté :
    // 1: Hippodamos (Facile) : tous les Quartiers sont niveau 1.
    // 2: Métagénès (Moyen) : tous les Quartiers sont niveau 1. Carrière = +2 pts.
    // 3: Callicratès (Difficile) : tous les Quartiers sont niveau 2.

    int multiplicateurNiveau = (difficulte == 3) ? 2 : 1;

    for (Tuile* t : tuilesAcquises) {
        for (size_t i = 0; i < t->getNbHexagones(); i++) {
            Hexagone* h = t->getHexagone(i);
            Type type = h->getType();

            if (type != Carriere) {
                // C'est un quartier (Habitation, Marché, Caserne, Temple, Jardin)
                // "tous les Quartiers ... sont considérés comme respectant correctement la condition de placement"
                // Donc chaque quartier rapporte des points comme s'il était bien placé.
                // En général, un quartier bien placé rapporte :
                // Habitation : 1 pt (si adjacente à d'autres, mais ici "respecte condition" = 1 pt ?)
                // Marché : 1 pt (si pas adjacent à d'autres marchés)
                // Caserne : 1 pt (si en périphérie)
                // Temple : 1 pt (si entouré) -> Non, Temple c'est 1 pt par défaut ?
                // Jardin : 1 pt (toujours)
                
                score += 1 * multiplicateurNiveau;
            }
            
            if (difficulte == 2 && type == Carriere) {
                score += 2;
            }
        }
    }

    return score;
}
