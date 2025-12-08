#include "score.h"
#include "partie.h"
#include "joueur.h"
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
                if (v->getType() == Marche) { isole = false; break; }
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
                if (v->getType() == Marche) isole = false;
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
    return RegleJardin::calculer(cite);
}


// =========================================================
// CLASSE SCORE
// =========================================================

Score::Score(Joueur* j) : joueur(j) {
    pointsParType[Habitation] = 0;
    pointsParType[Marche] = 0;
    pointsParType[Caserne] = 0;
    pointsParType[Temple] = 0;
    pointsParType[Jardin] = 0;

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
    total = j->getPierres();
}

Score::~Score() {
    for (auto& pair : strategies) delete pair.second;
    strategies.clear();
}

int Score::getScoreType(TypeQuartier type) const {
    auto it = pointsParType.find(type);
    return (it != pointsParType.end()) ? it->second : 0;
}

void Score::calculerScore() {
    Cite* maCite = joueur->getCite();
    if (!maCite) {
        total = 0;
        return;
    }

    total = 0;
    for (auto const& pair : strategies) {
        TypeQuartier type = pair.first;
        RegleScore* strategie = pair.second;
        if (strategie) {
            int pts = strategie->calculer(maCite);
            pointsParType[type] = pts;
            total += pts;
        }
    }
    total += joueur->getPierres();
}