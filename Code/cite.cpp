#include <iostream>
#include "cite.h"
#include "score.h"
#include "tuiles.h"
using namespace std;


vector<Cite::Coord> Cite::getVecteursVoisins() {
    return {
        {0, 1, 0}, {0, -1, 0},   // Vertical
        {1, 0, 0}, {-1, 0, 0},   // Horizontal
        {1, 1, 0}, {1, -1, 0},   // Diagonales Droite
        {-1, 1, 0}, {-1, -1, 0}  // Diagonales Gauche
    };
}

const bool Cite::toucheCite(Coord c) {
    // On vérifie large (carré de 3x3) pour gérer tous les cas d'adjacence
    for (const auto& vec : getVecteursVoisins()) {
        if (!estLibre({ c.x + vec.x, c.y + vec.y, c.z })) return true;
    }
    return false;
}

// =========================================================
// 2. MÉTHODE PLACER
// =========================================================

void Cite::placer(Tuile* t, Coord c) {
    // A. CALCUL DES POSITIONS (SLOTS)
    // Selon vos nouvelles règles strictes :
    Coord pos[3];

    // Index 0 : L'Ancre (Nord) -> x, y
    pos[0] = c;

    // Index 1 : Le Sud -> x, y+1
    pos[1] = { c.x, c.y + 1, c.z };

    // Index 2 : Le Côté -> x-1 (Standard) ou x+1 (Inversé), y+1
    if (t->getInversion()) {
        // Cas Inversé : x+1, y+1
        pos[2] = { c.x + 1, c.y + 1, c.z };
    }
    else {
        // Cas Standard : x-1, y+1
        pos[2] = { c.x - 1, c.y + 1, c.z };
    }

    // B. VÉRIFICATION : DISPONIBILITÉ
    for (int i = 0; i < 3; i++) {
        if (!estLibre(pos[i]))
            throw CiteException("Placement impossible : Une des cases n'est pas libre.");
    }

    // C. VÉRIFICATION : RÈGLES DE POSÉ
    if (c.z == 0) {
        // Au sol : Doit toucher la cité
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
        // En hauteur : Support requis sous CHAQUE hexagone
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
// 3. TUILE DE DÉPART & AUTRES
// =========================================================

void Cite::placerTuileDepart() {
    if (t->getNbHexagones() < 4) return;

    // Coordonnées spécifiques pour la tuile de départ (fixées précédemment)
    Coord c0 = { 0, 0, 0 };   // Centre (H0)
    Coord c1 = { -1, -1, 0 }; // Sud
    Coord c2 = { 1, -1, 0 };  // Autre
    Coord c3 = { 0, 2, 0 };   // Nord

    carte[c0] = t->getHexagone(0);
    carte[c1] = t->getHexagone(1);
    carte[c2] = t->getHexagone(2);
    carte[c3] = t->getHexagone(3);
}


// Helpers
Cite::Coord Cite::Coord::cote(bool inversion) {
    if (inversion) return { x + 1, y + 1, z };
    return { x - 1, y + 1, z };
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
        cout << "(" << paire.first.x << ", " << paire.first.y << ", " << paire.first.z << ")" << endl;
    }
}
	


void Cite::afficher() const{
    cout << quadrillage << endl;
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

