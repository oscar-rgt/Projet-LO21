#include <iostream>
#include "cite.h"
#include "score.h"
#include "tuiles.h"
using namespace std;



// =========================================================
// 1. OUTILS DE VOISINAGE
// =========================================================

// On garde une vérification large (8 voisins) pour être sûr de détecter
// les contacts quelle que soit la topologie de la grille.
vector<Cite::Coord> Cite::getVecteursVoisins() {
    return {
        {0, -1, 0}, {0, 1, 0},   // Vertical
        {-1, 0, 0}, {1, 0, 0},   // Horizontal bas
        {-1, 1, 0}, {1, 1, 0},   // Horizontal haut
    };
}

const bool Cite::toucheCite(Coord c) {
    for (const auto& vec : getVecteursVoisins()) {
        if (!estLibre({ c.x + vec.x, c.y + vec.y, c.z })) return true;
    }
    return false;
}

// =========================================================
// 2. MÉTHODE PLACER (Version Finale)
// =========================================================

void Cite::placer(Tuile* t, Coord c) {
    // A. CALCUL DES POSITIONS (SLOTS)
    // Nouvelle géométrie demandée :
    Coord pos[3];

    // Index 0 : L'Ancre (x, y)
    pos[0] = c;

    // Index 1 : (x, y-1)
    pos[1] = { c.x, c.y - 1, c.z };

    // Index 2 : (Standard: x-1, y) | (Inversé: x+1, y)
    if (t->getInversion()) {
        pos[2] = { c.x + 1, c.y, c.z };
    }
    else {
        pos[2] = { c.x - 1, c.y, c.z };
    }

    // B. VÉRIFICATION : DISPONIBILITÉ
    for (int i = 0; i < 3; i++) {
        if (!estLibre(pos[i]))
            throw CiteException("Placement impossible : Une des cases n'est pas libre.");
    }

    // C. VÉRIFICATION : RÈGLES DE JEU
    if (c.z == 0) {
        // Niveau 0 : Doit toucher la cité
        bool contact = false;
        for (int i = 0; i < 3; i++) {
            if (toucheCite(pos[i])) {
                contact = true;
                break;
            }
        }
        if (!contact) throw CiteException("Placement impossible : La tuile ne touche pas la cité.");
    }
    else {
        // Niveau Z > 0 : Support requis sous CHAQUE hexagone
        Tuile* supportTiles[3] = { nullptr, nullptr, nullptr };

        for (int i = 0; i < 3; i++) {
            Coord dessous = { pos[i].x, pos[i].y, pos[i].z - 1 };

            if (estLibre(dessous))
                throw CiteException("Placement impossible : Vide sous un hexagone.");

            supportTiles[i] = carte[dessous]->getTuile();
        }

        // Règle : Doit reposer sur au moins 2 tuiles différentes
        bool memeSupportPartout = (supportTiles[0] == supportTiles[1]) &&
            (supportTiles[1] == supportTiles[2]);

        if (memeSupportPartout)
            throw CiteException("Placement impossible : La tuile recouvre une seule et même tuile.");
    }

    // D. ENREGISTREMENT
    for (int i = 0; i < 3; i++) {
        carte[pos[i]] = t->getHexagone(i);
    }

    // E. AFFICHAGE
    remplirQuadrillage(c, *t);
}

// =========================================================
// 3. TUILE DE DÉPART
// =========================================================

void Cite::placerTuileDepart() {
    if (t->getNbHexagones() < 4) return;

    // Configuration personnalisée (selon votre code précédent)
    Coord c0 = { 0, 0, 0 };   // Centre
    Coord c1 = { -1, 0, 0 }; // Sud-Ouest ?
    Coord c2 = { 1, 0, 0 };  // Sud-Est ?
    Coord c3 = { 0, 1, 0 };   // Nord ?

    carte[c0] = t->getHexagone(0);
    carte[c1] = t->getHexagone(1);
    carte[c2] = t->getHexagone(2);
    carte[c3] = t->getHexagone(3);
}

void Cite::afficher() const {
    cout << quadrillage << endl;
}



// =========================================================
// 5. HELPERS
// =========================================================

// Mise à jour selon votre nouvelle géométrie "Index 2"
Cite::Coord Cite::Coord::cote(bool inversion) {
    if (inversion) return { x + 1, y, z };
    return { x - 1, y, z };
}

vector<Hexagone*> Cite::getAdjacents(Coord c) {
    vector<Hexagone*> ret;
    for (const auto& vec : getVecteursVoisins()) {
        Coord voisin = { c.x + vec.x, c.y + vec.y, c.z };
        if (!estLibre(voisin)) ret.push_back(carte[voisin]);
    }
    return ret;
}

void Cite::afficherMap() const {
    for (const auto& paire : carte) {
        cout << "(" << paire.first.x << ", "
            << paire.first.y << ", "
            << paire.first.z << ")" << endl;
    }
}

void Cite::remplirQuadrillage(Coord c, Tuile& t) {
    for (int i = 0; i < 3; i++) {
        int j = 0;
        Coord h;
        // bonnes coordonnees selon l'hexa
        if (i == 1) {
            h = c.cote(t.getInversion());
        }
        else if (i == 2) {
            h = c.sud();
        }
        else {
            h = c;
        }
        // placement
        j += 109 * (h.x+1) * 2;
        if ((h.x+1) % 2 == 0) {
            j += 3 + 14 * h.y;
        }
        else {
            j += 10 + 14 * h.y;
        }
        quadrillage.replace(j, 3, t.getHexagone(i)->affiche());
    }
}

