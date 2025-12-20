#include "score.h"
#include "partie.h"
#include "joueur.h"
#include "IA.h" 
#include "cite.h" 
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

// =========================================================
// OUTILS COMMUNS
// =========================================================

int RegleScore::compterEtoiles(Cite* cite, TypeQuartier typeQ) const {
    int etoiles = 0;
    // Boucle via itérateur
    for (auto it = cite->begin(); it != cite->end(); ++it) {
        Hexagone* h = it->second;
        if (h && h->estPlace() && h->getType() == typeQ) {
            etoiles += h->getEtoiles();
        }
    }
    return etoiles;
}

// =========================================================
// 1. HABITATIONS (Bleu)
// =========================================================

int RegleHabitation::getValeurPlusGrandGroupe(Cite* cite) const {
    if (cite->begin() == cite->end()) return 0; // Test vide

    vector<Coord> globalVisites;
    int maxValeur = 0;

    for (auto it = cite->begin(); it != cite->end(); ++it) {
        Coord depart = it->first;
        Hexagone* h = it->second;

        // Vérification si déjà visité
        bool dejaVu = false;
        for (const auto& v : globalVisites) { if (v == depart) { dejaVu = true; break; } }

        if (h->getType() == Habitation && !h->estPlace() && !dejaVu) {

            // --- DÉBUT PARCOURS GROUPE ---
            int valeurGroupe = 0;
            vector<Coord> aTraiter;

            aTraiter.push_back(depart);
            globalVisites.push_back(depart);

            // Parcours en largeur (BFS)
            for (size_t i = 0; i < aTraiter.size(); ++i) {
                Coord actuelle = aTraiter[i];
                valeurGroupe += (actuelle.z + 1);

                auto voisinsVecteurs = cite->getVecteursVoisins(actuelle.x%2);

                for (const auto& vec : voisinsVecteurs) {
                    for (int dz = -1; dz <= 1; ++dz) {
                        if (actuelle.z + dz < 0) continue;

                        Coord voisinCoord = { actuelle.x + vec.x, actuelle.y + vec.y, actuelle.z + dz };

                        
                        const Hexagone* hVoisin = cite->getHexagone(voisinCoord);

                        if (hVoisin) {
                            bool voisinDejaVu = false;
                            for (const auto& v : globalVisites) { if (v == voisinCoord) { voisinDejaVu = true; break; } }

                            if (hVoisin->getType() == Habitation && !hVoisin->estPlace() && !voisinDejaVu) {
                                aTraiter.push_back(voisinCoord);
                                globalVisites.push_back(voisinCoord);
                            }
                        }
                    }
                }
            }
            if (valeurGroupe > maxValeur) maxValeur = valeurGroupe;
        }
    }
    return maxValeur;
}

int RegleHabitation::calculer(Cite* cite) const {
    return getValeurPlusGrandGroupe(cite) * compterEtoiles(cite, Habitation);
}

int RegleHabitationVariante::calculer(Cite* cite) const {
    int valeur = getValeurPlusGrandGroupe(cite);
    int score = valeur * compterEtoiles(cite, Habitation);
    if (valeur >= 10) score *= 2;
    return score;
}

// =========================================================
// 2. MARCHÉS (Jaune)
// =========================================================

int RegleMarche::calculer(Cite* cite) const {
    int valeur = 0;
    for (auto it = cite->begin(); it != cite->end(); ++it) {
        Hexagone* h = it->second;
        if (h->getType() == Marche && !h->estPlace()) {
            bool isole = true;
            auto voisins = cite->getAdjacents(it->first);
            for (Hexagone* v : voisins) {
                if (v->getType() == Marche && !v->estPlace()) { isole = false; break; }
            }
            if (isole) valeur += (it->first.z + 1);
        }
    }
    return valeur * compterEtoiles(cite, Marche);
}

int RegleMarcheVariante::calculer(Cite* cite) const {
    int total = 0;
    int etoiles = compterEtoiles(cite, Marche);

    for (auto it = cite->begin(); it != cite->end(); ++it) {
        Hexagone* h = it->second;
        if (h->getType() == Marche && !h->estPlace()) {
            bool isole = true;
            bool adjacentPlace = false;
            auto voisins = cite->getAdjacents(it->first);
            for (Hexagone* v : voisins) {
                if (v->getType() == Marche && !v->estPlace()) isole = false;
                if (v->estPlace() && v->getType() == Marche) adjacentPlace = true;
            }
            if (isole) {
                int pts = (it->first.z + 1) * etoiles;
                if (adjacentPlace) pts *= 2;
                total += pts;
            }
        }
    }
    return total;
}

// =========================================================
// 3. CASERNES (Rouge)
// =========================================================

int RegleCaserne::calculer(Cite* cite) const {
    int valeur = 0;
    for (auto it = cite->begin(); it != cite->end(); ++it) {
        Hexagone* h = it->second;
        if (h->getType() == Caserne && !h->estPlace()) {
            if (cite->getAdjacents(it->first).size() < 6) {
                valeur += (it->first.z + 1);
            }
        }
    }
    return valeur * compterEtoiles(cite, Caserne);
}

int RegleCaserneVariante::calculer(Cite* cite) const {
    int total = 0;
    int etoiles = compterEtoiles(cite, Caserne);

    for (auto it = cite->begin(); it != cite->end(); ++it) {
        Hexagone* h = it->second;
        if (h->getType() == Caserne && !h->estPlace()) {
            size_t nbVoisins = cite->getAdjacents(it->first).size();
            if (nbVoisins < 6) {
                int pts = (it->first.z + 1) * etoiles;
                int vides = 6 - (int)nbVoisins;
                if (vides == 3 || vides == 4) pts *= 2;
                    total += pts;
                }
        }
    }
    return total;
}

// =========================================================
// 4. TEMPLES (Violet)
// =========================================================

int RegleTemple::calculer(Cite* cite) const {
    int valeur = 0;
    for (auto it = cite->begin(); it != cite->end(); ++it) {
        Hexagone* h = it->second;
        if (h->getType() == Temple && !h->estPlace()) {
            if (cite->getAdjacents(it->first).size() == 6) {
                valeur += (it->first.z + 1);
            }
        }
    }
    return valeur * compterEtoiles(cite, Temple);
}

int RegleTempleVariante::calculer(Cite* cite) const {
    int total = 0;
    int etoiles = compterEtoiles(cite, Temple);

    for (auto it = cite->begin(); it != cite->end(); ++it) {
        Hexagone* h = it->second;
        if (h->getType() == Temple && !h->estPlace()) {
            if (cite->getAdjacents(it->first).size() == 6) {
                int pts = (it->first.z + 1) * etoiles;
                if (it->first.z > 0) pts *= 2;
                total += pts;
            }
        }
    }
    return total;
}

// =========================================================
// 5. JARDINS (Vert)
// =========================================================

int RegleJardin::calculer(Cite* cite) const {
    int valeur = 0;
    for (auto it = cite->begin(); it != cite->end(); ++it) {
        Hexagone* h = it->second;
        if (h->getType() == Jardin && !h->estPlace()) {
            valeur += (it->first.z + 1);
        }
    }
    return valeur * compterEtoiles(cite, Jardin);
}

int RegleJardinVariante::calculer(Cite* cite) const {
    int total = 0;
    int etoiles = compterEtoiles(cite, Jardin);

    for (auto it = cite->begin(); it != cite->end(); ++it) {
        Hexagone* h = it->second;
        if (h->getType() == Jardin && !h->estPlace()) {
            int pts = (it->first.z + 1) * etoiles;

            // Vérifier si adjacent à un lac (espace vide complètement entouré)
            bool adjacentLac = false;
            auto voisinsVecteurs = cite->getVecteursVoisins(it->first.x % 2);

            for (const auto& vec : voisinsVecteurs) {
                for (int dz = -1; dz <= 1; ++dz) {
                    if (it->first.z + dz < 0) continue;

                    Coord voisinCoord = { it->first.x + vec.x, it->first.y + vec.y, it->first.z + dz };

                    // Vérifier si c'est un lac : position vide + 6 voisins
                    if (cite->getHexagone(voisinCoord) == nullptr) {
                        auto voisinsLac = cite->getAdjacents(voisinCoord);
                        if (voisinsLac.size() == 6) {
                            adjacentLac = true;
                            break;
                        }
                    }
                }
                if (adjacentLac) break;
            }

            if (adjacentLac) pts *= 2;
            total += pts;
        }
    }
    return total;
}

// =========================================================
// CLASSE SCORE
// =========================================================

Score::Score(Joueur* j) : joueur(j) {

    const auto& vars = Partie::getInstance().getVariantes();

    if (vars[0]) strategies[Habitation] = new RegleHabitationVariante();
    else         strategies[Habitation] = new RegleHabitation();

    if (vars[1]) strategies[Marche] = new RegleMarcheVariante();
    else         strategies[Marche] = new RegleMarche();

    if (vars[2]) strategies[Caserne] = new RegleCaserneVariante();
    else         strategies[Caserne] = new RegleCaserne();

    if (vars[3]) strategies[Temple] = new RegleTempleVariante();
    else         strategies[Temple] = new RegleTemple();

    if (vars[4]) strategies[Jardin] = new RegleJardinVariante();
    else         strategies[Jardin] = new RegleJardin();
}

Score::~Score() {
    for (auto& pair : strategies) delete pair.second;
    strategies.clear();
}

int Score::calculerScore() const {
    if (!joueur) return 0;
    int totalCalcule = 0;
    if(IA* ia = dynamic_cast<IA*>(joueur)) {
		
        // Maps pour stocker les comptes séparés
        // nbQuartiers : compte uniquement les tuiles "Quartier" (pas les places)
        // nbEtoiles : compte uniquement les étoiles des "Places"
        map<TypeQuartier, int> nbQuartiers;
        map<TypeQuartier, int> nbEtoiles;
        int nbCarrieres = 0;

        // --- 1. INVENTAIRE ---
        for (auto it = ia->begin(); it != ia->end(); ++it) {
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
        int hauteur = (ia->getDifficulte() == 3) ? 2 : 1;

        // --- 3. CALCUL DU SCORE ---
        // On parcourt les types de quartiers rencontrés
        for (map<TypeQuartier, int>::iterator it = nbQuartiers.begin(); it != nbQuartiers.end(); ++it) {
            TypeQuartier type = it->first;
            int nombreDeQuartiers = it->second; // Nombre d'hexagones "non-place"
            int nombreEtoiles = nbEtoiles[type]; // Total des étoiles accumulées

            // Formule : Quartiers x Etoiles x Hauteur
            totalCalcule += nombreDeQuartiers * nombreEtoiles * hauteur;
        }

        // --- 4. POINTS DES PIERRES ---
        totalCalcule += joueur->getPierres();

        // --- 5. BONUS MÉTAGÉNÈS (Difficulté 2) ---
        if (ia->getDifficulte() == 2) {
            totalCalcule += nbCarrieres * 2;
        }
	}
    else {
        Cite* maCite = joueur->getCite();
        if (!maCite) return 0;

        // 1. On re-parcourt les règles (C'est très rapide pour un ordi)
        for (auto const& pair : strategies) {
            if (pair.second) {
                totalCalcule += pair.second->calculer(maCite);
            }
        }

        // 2. On ajoute les pierres (Toujours aller chercher la vraie valeur chez le joueur)
        totalCalcule += joueur->getPierres();
    }
    

    return totalCalcule;
}
