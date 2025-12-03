#include <iostream>
#include "cite.h"
#include "score.h"
#include "tuiles.h"
#include <cmath>
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
        {-1, 1, 0}, {1, 1, 0}  // Horizontal haut
    };
}

const bool Cite::toucheCite(Coord c) { //marche pas (jsp pk)
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
    pos[1] = c.sud();

    // Index 2 : (Standard: x-1, x % 2 == 0 ? y : y - 1) | (Inversé: x+1, x % 2 == 0 ? y : y - 1)
    pos[2] = c.cote(t->getInversion());

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
    // Temporaire : sortie du cadrillage
    for (Coord h : pos) if (h.x > 7 || h.x < -7 || h.y>3 || h.y < -2 || h.z < 0) throw CiteException("Placement impossible : Quadrillage trop petit (WIP)");

    //D.Sauvegarde
    Action act;
    act.tuileId = t->getId();
    act.x = c.x;
    act.y = c.y;
    act.z = c.z;
    act.inversion = t->getInversion();

    // On sauvegarde l'état VISUEL actuel des hexagones (après rotation éventuelle)
    for (int k = 0; k < 3; k++) {
        Hexagone* h = t->getHexagone(k);
        act.hexas[k].type = (int)h->getType();
        act.hexas[k].etoiles = h->getEtoiles();
    }
    historique.push_back(act);

    // E. ENREGISTREMENT
    for (int i = 0; i < 3; i++) {
        carte[pos[i]] = t->getHexagone(i);
    }
    // F. AFFICHAGE
    remplirQuadrillage(c, *t);
}

// =========================================================
// 3. TUILE DE DÉPART
// =========================================================

void Cite::placerTuileDepart() {
    if (t->getNbHexagones() != 4) throw CiteException("La tuile de départ doit avoir 4 hexagones.");

    // Centre (Habitation) en (0,0,0)
    Coord c0 = { 0, 0, 0 };


    // placer autour de (0,0,0)    
    Coord c1 = { -1, 0, 0 };
    Coord c2 = { 1, 0, 0 };
    Coord c3 = { 0, 1, 0 }; //NORD

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
    if (inversion) return { x + 1, x % 2 == 0 ? y : y - 1, z };
    return { x - 1, x % 2 == 0 ? y : y - 1, z };
}

vector<Hexagone*> Cite::getAdjacents(Coord c) {
    vector<Hexagone*> ret;
    for (const auto& vec : getVecteursVoisins()) {
        Coord voisin = { c.x + vec.x, c.y + vec.y, c.z };
        if (!estLibre(voisin)) ret.push_back(carte[voisin]);
    }
    return ret;
}

void Cite::remplirQuadrillage(Coord c, Tuile& t) {
    for (int i = 0; i < 3; i++) {
        int j = 0;
        Coord h;
        // bonnes coordonnees selon l'hexa
        if (i == 1) {
            h = c.sud();
        }
        else if (i == 2) {
            h = c.cote(t.getInversion());
        }
        else {
            h = c;
        }

        int l = h.y * -4 + 14;
        int c = h.x * 7 + 56;
        j = c + l * 110;
        if ((h.x % 2)) j += 220;
        if (j<0 || j > quadrillage.length()) throw CiteException("Placement impossible : sortie du quadrillage");
        quadrillage.replace(j, 3, t.getHexagone(i)->affiche());
    }
}





void Cite::agrandirQ(char dir) {
    if (dir == 'S') {
        string rep = to_string(lround(quadrillage.length() / 110) - 30);
        rep += R"(/     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \  )";
        quadrillage.replace(quadrillage.length() - 110, 110, rep);
        quadrillage += R"(
 /       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \ 
 \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       / 
  \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/  
     -7     -6     -5    -4     -3     -2     -1      0      1      2      3      4      5      6      7     
)";
    }
}
