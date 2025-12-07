#include <iostream>
#include "cite.h"
#include "score.h"
#include "tuiles.h"
#include "joueur.h"
#include <cmath>
using namespace std;

const Hexagone* Cite::getHexagone(Coord c) const {
    auto it = carte.find(c);
    if (it != carte.end()) return it->second;
    return nullptr;
}

// =========================================================
// 1. OUTILS DE VOISINAGE
// =========================================================

// On garde une vérification large (8 voisins) pour être sûr de détecter
// les contacts quelle que soit la topologie de la grille.
vector<Coord> Cite::getVecteursVoisins(bool isXOdd) const{
    if (!isXOdd) return {
        {0, -1, 0}, {0, 1, 0},   // Vertical
        {-1, 0, 0}, {1, 0, 0},   // Horizontal même ligne
        {-1, 1, 0}, {1, 1, 0}  // Horizontal autre ligne
    };
    return {
            {0, -1, 0}, {0, 1, 0},   // Vertical
            {-1, 0, 0}, {1, 0, 0},   // Horizontal même ligne
            {-1, -1, 0}, {1, -1, 0}  // Horizontal autre ligne
    };
}

const bool Cite::toucheCite(Coord c) { //marche pas (jsp pk)
    for (const auto& vec : getVecteursVoisins((c.x)%2)) {
        if (!estLibre({ c.x + vec.x, c.y + vec.y, c.z })) return true;
    }
    return false;
}

// =========================================================
// 2. MÉTHODE PLACER (Version Finale)
// =========================================================

void Cite::placer(Tuile* t, Coord c, Joueur* j) {
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
        if (!contact) throw CiteException("Placement impossible : La tuile ne touche pas la cite.");
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
    for (Coord h : pos) if (h.x > 7 || h.x < -7 || h.y>3 || h.y < -99 || h.z < 0) throw CiteException("Placement impossible : Quadrillage trop petit (WIP)");

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
        if (c.z > 0) {
			t->getHexagone(i)->setNiveau(c.z);
            Coord dessous = { pos[i].x, pos[i].y, pos[i].z - 1 };
            auto it = carte.find(dessous);
            if (it != carte.end()) {
				if (it->second->getType() == TypeQuartier::Carriere) j->ajouterPierres(1); //donne 1 pierre si hexagone en dessous est une place
                carte.erase(it);
            }
        }
        carte[pos[i]] = t->getHexagone(i);
    }
    // F. AFFICHAGE
    if (pos[0].y < (-lround((quadrillage.length() / 111) / 4) + 5)) agrandirQ('S');
    if (pos[1].y<( - lround((quadrillage.length() / 111) / 4) + 5)) agrandirQ('S');
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
Coord Coord::cote(bool inversion) {
    if (inversion) return { x + 1, x % 2 == 0 ? y : y - 1, z };
    return { x - 1, x % 2 == 0 ? y : y - 1, z };
}

vector<Hexagone*> Cite::getAdjacents(Coord c) const {
    vector<Hexagone*> ret;
    for (const auto& vec : getVecteursVoisins(c.x%2)) {
        Coord voisin = { c.x + vec.x, c.y + vec.y, c.z };
        auto it = carte.find(voisin);

        // Si l'itérateur n'est pas à la fin, c'est que l'élément existe
        if (it != carte.end()) {
            ret.push_back(it->second); // it->second donne la valeur (Hexagone*), obligé d'utliser ça car méthode const
        }
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
        int line_offset = 14;
        int hex_height = 4;
        int hex_width = 7;
        int col_offset = 56;
        int l = h.y * - hex_height + line_offset;
        int c = h.x * hex_width + col_offset;
        j = c + l * 111 - lround(h.y/1.5);
        if ((h.x % 2)) j += 222;
        if (j<0 || j > quadrillage.length()) throw CiteException("Placement impossible : sortie du quadrillage");
        quadrillage.replace(j, 3, t.getHexagone(i)->affiche());
    }
}





void Cite::agrandirQ(char dir) {
    if (dir == 'S') {
        int row_num = -lround((quadrillage.length() / 111)/4) +4;
        string rep = to_string(row_num);
        if (row_num > -10) rep += " ";
        rep += R"(/     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \   )";
        quadrillage.replace(quadrillage.length() - 111, 111, rep);
        quadrillage += R"(
  /       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \ 
  \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       / 
   \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/  
    -7     -6     -5     -4     -3     -2     -1      0      1      2      3      4      5      6      7       )";
    }
}
