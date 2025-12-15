#include "ia.h"
#include "tuiles.h"
#include "hexagone.h"
#include "partie.h"
#include<map>
using namespace std;



IA::~IA() {
    tuilesAcquises.clear();
}

void IA::ajouterTuile(Tuile* t) {
    tuilesAcquises.push_back(t);
}

bool IA::tuileContientPlace(Tuile* t) {
    if (!t) return false;
    for (int i = 0; i < t->getNbHexagones(); ++i) {
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
    int scoreTotal = 0;

    // Maps pour stocker les comptes (Type -> Nombre)
    map<TypeQuartier, int> nbHexagones;
    map<TypeQuartier, int> nbEtoiles;
    int nbCarrieres = 0;

    // --- 1. INVENTAIRE TUILES ACQUISES ---
    for (auto it = begin(); it != end(); ++it) {
        Tuile* t = *it; 

        for (int i = 0; i < t->getNbHexagones(); ++i) {
            Hexagone* h = t->getHexagone(i);
            TypeQuartier type = h->getType();

            if (type == Carriere) {
                nbCarrieres++;
            }
            else {
                if (nbHexagones.find(type) == nbHexagones.end()) {
                    nbHexagones[type] = 0;
                    nbEtoiles[type] = 0;
                }

                // On incrémente les compteurs
                nbHexagones[type]++;
                nbEtoiles[type] += h->getEtoiles();
            }
        }
    }

    // --- 2. RÈGLES DE DIFFICULTÉ (HAUTEUR) ---
    // Hippodamos (1) & Métagénès (2) : Hauteur = 1
    // Callicratès (3) : Hauteur = 2 (Tous les quartiers comptent double)
    int hauteur = (difficulte == 3) ? 2 : 1;

    // --- 3. CALCUL FINAL ---
    for (map<TypeQuartier, int>::iterator it = nbHexagones.begin(); it != nbHexagones.end(); ++it) {
        TypeQuartier type = it->first;
        int count = it->second;
        int stars = nbEtoiles[type]; // On récupère le nombre d'étoiles associées

        // Formule : (Nombre de Tuiles) * (Nombre d'Étoiles) * (Hauteur)
        scoreTotal += count * stars * hauteur;
    }

    // --- 4. POINTS DES PIERRES ---
    scoreTotal += getPierres();

    // --- 5. BONUS MÉTAGÉNÈS (Difficulté 2) ---
    // Il gagne 2 points par carrière
    if (difficulte == 2) {
        scoreTotal += nbCarrieres * 2;
    }

    return scoreTotal;
}