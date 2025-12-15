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

    // Maps pour stocker les comptes séparés
    // nbQuartiers : compte uniquement les tuiles "Quartier" (pas les places)
    // nbEtoiles : compte uniquement les étoiles des "Places"
    map<TypeQuartier, int> nbQuartiers;
    map<TypeQuartier, int> nbEtoiles;
    int nbCarrieres = 0;

    // --- 1. INVENTAIRE ---
    for (auto it = begin(); it != end(); ++it) {
        Tuile* t = *it;

        for (int i = 0; i < t->getNbHexagones(); ++i) {
            Hexagone* h = t->getHexagone(i);
            TypeQuartier type = h->getType();

            if (type == Carriere) {
                nbCarrieres++;
            }
            else {
                if (nbQuartiers.find(type) == nbQuartiers.end()) nbQuartiers[type] = 0;
                if (nbEtoiles.find(type) == nbEtoiles.end()) nbEtoiles[type] = 0;

                if (h->estPlace()) {
                    nbEtoiles[type] += h->getEtoiles();
                }
                else {
                    nbQuartiers[type]++;
                }
            }
        }
    }

    // --- 2. RÈGLES DE DIFFICULTÉ (HAUTEUR) ---
    // Hippodamos (1) & Métagénès (2) : Hauteur = 1
    // Callicratès (3) : Hauteur = 2
    int hauteur = (difficulte == 3) ? 2 : 1;

    // --- 3. CALCUL DU SCORE ---
    // On parcourt les types de quartiers rencontrés
    for (map<TypeQuartier, int>::iterator it = nbQuartiers.begin(); it != nbQuartiers.end(); ++it) {
        TypeQuartier type = it->first;
        int nombreDeQuartiers = it->second; // Nombre d'hexagones "non-place"
        int nombreEtoiles = nbEtoiles[type]; // Total des étoiles accumulées

        // Formule : Quartiers x Etoiles x Hauteur
        scoreTotal += nombreDeQuartiers * nombreEtoiles * hauteur;
    }

    // --- 4. POINTS DES PIERRES ---
    scoreTotal += getPierres();

    // --- 5. BONUS MÉTAGÉNÈS (Difficulté 2) ---
    if (difficulte == 2) {
        scoreTotal += nbCarrieres * 2;
    }

    return scoreTotal;
}