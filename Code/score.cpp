#include "score.h"
#include "partie.h"
#include "joueur.h"
#include "cite.h" // Nécessaire ici pour voir la définition complète de Cite
#include <vector>
#include <iostream>
#include <algorithm> // pour std::find

using namespace std;

// =========================================================
// OUTILS COMMUNS
// =========================================================

int RegleScore::compterEtoiles(Cite* cite, TypeQuartier typeQ) const {
    int etoiles = 0;
    // On parcourt la carte
    // Note : On utilise auto car on ne connait pas forcément le type exact de l'itérateur si Coord est privé
    for (const auto& it : cite->getCarte()) {
        Hexagone* h = it.second;
        if (h && h->estPlace() && h->getType() == typeQ) {
            etoiles += h->getEtoiles();
        }
    }
    return etoiles;
}

// =========================================================
// 1. HABITATIONS (Bleu) - Algorithme Itératif
// =========================================================

int RegleHabitation::getValeurPlusGrandGroupe(Cite* cite) const {
    // On utilise auto& pour ne pas avoir à écrire CoordMap::const_iterator...
    const auto& carte = cite->getCarte();
    if (carte.empty()) return 0;

    // On stocke les coordonnées visitées. 
    // Attention: Cela nécessite que Coord soit accessible ou que RegleHabitation soit friend.
    vector<Coord> globalVisites;
    int maxValeur = 0;

    for (const auto& it : carte) {
        Coord depart = it.first;
        Hexagone* h = it.second;

        // Vérifier si déjà visité (sans std::set pour faire simple)
        bool dejaVu = false;
        for (const auto& v : globalVisites) { if (v == depart) { dejaVu = true; break; } }

        if (h->getType() == Habitation && !h->estPlace() && !dejaVu) {

            // --- DÉBUT PARCOURS GROUPE ---
            int valeurGroupe = 0;
            vector<Coord> aTraiter;

            aTraiter.push_back(depart);
            globalVisites.push_back(depart);

            // On utilise un index pour parcourir le vecteur comme une file
            for (size_t i = 0; i < aTraiter.size(); ++i) {
                Coord actuelle = aTraiter[i];

                // Valeur = niveau + 1 (selon règle)
                valeurGroupe += (actuelle.z + 1);

                // ICI : On appelle la méthode d'instance de la Cité !
                auto voisinsVecteurs = cite->getVecteursVoisins();

                for (const auto& vec : voisinsVecteurs) {
                    // On reconstruit la coordonnée voisine
                    Coord voisinCoord = { actuelle.x + vec.x, actuelle.y + vec.y, actuelle.z };

                    // On vérifie si ce voisin existe dans la carte
                    auto itVoisin = carte.find(voisinCoord);
                    if (itVoisin != carte.end()) {
                        Hexagone* hVoisin = itVoisin->second;

                        // Vérif déjà vu locale pour ce tour de boucle
                        bool voisinDejaVu = false;
                        for (const auto& v : globalVisites) { if (v == voisinCoord) { voisinDejaVu = true; break; } }

                        if (hVoisin->getType() == Habitation && !hVoisin->estPlace() && !voisinDejaVu) {
                            aTraiter.push_back(voisinCoord);
                            globalVisites.push_back(voisinCoord);
                        }
                    }
                }
            }
            // --- FIN GROUPE ---

            if (valeurGroupe > maxValeur) {
                maxValeur = valeurGroupe;
            }
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
    for (const auto& it : cite->getCarte()) {
        Hexagone* h = it.second;
        if (h->getType() == Marche && !h->estPlace()) {
            bool isole = true;
            // On utilise la méthode publique getAdjacents de Cite
            try {
                auto voisins = cite->getAdjacents(it.first);
                for (Hexagone* v : voisins) {
                    if (v->getType() == Marche) { isole = false; break; }
                }
            }
            catch (...) {}

            if (isole) valeur += (it.first.z + 1);
        }
    }
    return valeur * compterEtoiles(cite, Marche);
}

int RegleMarcheVariante::calculer(Cite* cite) const {
    int total = 0;
    int etoiles = compterEtoiles(cite, Marche);

    for (const auto& it : cite->getCarte()) {
        Hexagone* h = it.second;
        if (h->getType() == Marche && !h->estPlace()) {
            bool isole = true;
            bool adjacentPlace = false;
            try {
                auto voisins = cite->getAdjacents(it.first);
                for (Hexagone* v : voisins) {
                    if (v->getType() == Marche) isole = false;
                    if (v->estPlace() && v->getType() == Marche) adjacentPlace = true;
                }
            }
            catch (...) {}

            if (isole) {
                int pts = (it.first.z + 1) * etoiles;
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
    for (const auto& it : cite->getCarte()) {
        Hexagone* h = it.second;
        if (h->getType() == Caserne && !h->estPlace()) {
            try {
                // Moins de 6 voisins = périphérie
                if (cite->getAdjacents(it.first).size() < 6) {
                    valeur += (it.first.z + 1);
                }
            }
            catch (...) {}
        }
    }
    return valeur * compterEtoiles(cite, Caserne);
}

int RegleCaserneVariante::calculer(Cite* cite) const {
    int total = 0;
    int etoiles = compterEtoiles(cite, Caserne);

    for (const auto& it : cite->getCarte()) {
        Hexagone* h = it.second;
        if (h->getType() == Caserne && !h->estPlace()) {
            try {
                size_t nbVoisins = cite->getAdjacents(it.first).size();
                if (nbVoisins < 6) {
                    int pts = (it.first.z + 1) * etoiles;
                    // Variante : 3 ou 4 vides
                    int vides = 6 - (int)nbVoisins;
                    if (vides == 3 || vides == 4) pts *= 2;
                    total += pts;
                }
            }
            catch (...) {}
        }
    }
    return total;
}

// =========================================================
// 4. TEMPLES (Violet)
// =========================================================

int RegleTemple::calculer(Cite* cite) const {
    int valeur = 0;
    for (const auto& it : cite->getCarte()) {
        Hexagone* h = it.second;
        if (h->getType() == Temple && !h->estPlace()) {
            try {
                if (cite->getAdjacents(it.first).size() == 6) {
                    valeur += (it.first.z + 1);
                }
            }
            catch (...) {}
        }
    }
    return valeur * compterEtoiles(cite, Temple);
}

int RegleTempleVariante::calculer(Cite* cite) const {
    int total = 0;
    int etoiles = compterEtoiles(cite, Temple);

    for (const auto& it : cite->getCarte()) {
        Hexagone* h = it.second;
        if (h->getType() == Temple && !h->estPlace()) {
            try {
                if (cite->getAdjacents(it.first).size() == 6) {
                    int pts = (it.first.z + 1) * etoiles;
                    if (it.first.z > 0) pts *= 2;
                    total += pts;
                }
            }
            catch (...) {}
        }
    }
    return total;
}

// =========================================================
// 5. JARDINS (Vert)
// =========================================================

int RegleJardin::calculer(Cite* cite) const {
    int valeur = 0;
    for (const auto& it : cite->getCarte()) {
        Hexagone* h = it.second;
        if (h->getType() == Jardin && !h->estPlace()) {
            valeur += (it.first.z + 1);
        }
    }
    return valeur * compterEtoiles(cite, Jardin);
}

int RegleJardinVariante::calculer(Cite* cite) const {
    return RegleJardin::calculer(cite);
}


// =========================================================
// CLASSE SCORE (Le Chef d'orchestre)
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
    // Version classique (compatible vieux C++)
    for (auto const& pair : strategies) {
        TypeQuartier type = pair.first;       // La clé (TypeQuartier)
        RegleScore* strategie = pair.second;  // La valeur (La règle)

        if (strategie) {
            // Attention : ici on passe *maCite (valeur) car la méthode attend "const Cite&"
            // Si votre méthode attend "Cite*", mettez juste "maCite"
            int pts = strategie->calculer(maCite);

            pointsParType[type] = pts;
            total += pts;
        }
    }
    total += joueur->getPierres();
}