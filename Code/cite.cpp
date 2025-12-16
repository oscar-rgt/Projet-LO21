#include <iostream>
#include "cite.h"
#include "score.h"
#include "tuiles.h"
#include "joueur.h"
#include "coord.h"
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

const bool Cite::toucheCite(Coord c) const { 
    for (const auto& vec : getVecteursVoisins((c.x)%2)) {
        for (int z = 0; z <= getHauteurMax(); ++z) {
            if (!estLibre({ c.x + vec.x, c.y + vec.y, z })) {
                return true; // On a trouvé un morceau de cité adjacent (peu importe la hauteur)
            }
        }
    }
    return false;
}

// =========================================================
// 2. MÉTHODE PLACER (Version Finale)
// =========================================================

void Cite::placer(Tuile* t, Coord c, Joueur* j, int rotation) {
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
    // Sortie du cadrillage
    for (Coord h : pos) if (h.y>999 || h.y < -99 || h.z < 0) throw CiteException("Placement impossible : Quadrillage trop petit (WIP)");

    //D.Sauvegarde
    Action act;
    act.tuileId = t->getId();
    act.pos.x = c.x;
    act.pos.y = c.y;
    act.pos.z = c.z;
    act.inversion = t->getInversion();
    act.rotation = rotation;

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





// =========================================================
// 5. HELPERS
// =========================================================


vector<Hexagone*> Cite::getAdjacents(Coord c) const {
    vector<Hexagone*> ret;

    // Pour chaque direction autour de la tuile...
    for (const auto& vec : getVecteursVoisins(c.x % 2)) {
        for (int z = 0; z <= getHauteurMax(); ++z) { // on regarde toutes les hauteurs possibles
            Coord voisin = { c.x + vec.x, c.y + vec.y, z };
            auto it = carte.find(voisin);

            if (it != carte.end()) {
                ret.push_back(it->second);
                break; // On a trouvé le voisin visible, on passe à la direction suivante
            }
        }
    }
    return ret;
}


int Cite::getHauteurMax() const {
    int maxZ = 0;
    // On utilise votre itérateur const
    for (auto it = begin(); it != end(); ++it) {
        // it->first est la clé (Coord), it->second est la valeur (Hexagone*)
        int zActuel = it->first.z;
        if (zActuel > maxZ) {
            maxZ = zActuel;
        }
    }
    return maxZ;
}




