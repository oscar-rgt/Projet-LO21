#include "cite.h"
#include "score.h"
#include "tuiles.h"
#include "joueur.h"
#include "coord.h"

using namespace std;

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

void Cite::placer(Tuile* t, Coord c, Joueur* j, int rotation) {
	Coord pos[3]; //positions des 3 hexagones de la tuile

    //centre
    pos[0] = c;

    //sud
    pos[1] = c.sud();

    //coté : (Standard: x-1, x % 2 == 0 ? y : y - 1) | (Inversé: x+1, x % 2 == 0 ? y : y - 1)
   if (t->getInversion()) {
        //vers la droite
        pos[2] = { c.x + 1, (c.x % 2 == 0) ? c.y : c.y - 1, c.z };
    } else {
        //vers la gauche (standard)
        pos[2] = { c.x - 1, (c.x % 2 == 0) ? c.y : c.y - 1, c.z };
    }

    //verif disponibilité de la case
    for (int i = 0; i < 3; i++) {
        if (!estLibre(pos[i]))
            throw CiteException("Placement impossible : Une des cases n'est pas libre.");
    }

    //verifs règles du jeu :
    if (c.z == 0) {
        //Niveau 0 : doit toucher la cité
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
        //niveau z > 0 : Support requis sous CHAQUE hexagone
        Tuile* supportTiles[3] = { nullptr, nullptr, nullptr };

        for (int i = 0; i < 3; i++) {
            Coord dessous = { pos[i].x, pos[i].y, pos[i].z - 1 };

            if (estLibre(dessous))
                throw CiteException("Placement impossible : Il y a du vide sous un hexagone.");

            supportTiles[i] = carte[dessous]->getTuile();
        }

        //règle : doit reposer sur au moins 2 tuiles différentes
        bool memeSupportPartout = (supportTiles[0] == supportTiles[1]) &&
            (supportTiles[1] == supportTiles[2]);

        if (memeSupportPartout)
            throw CiteException("Placement impossible : La tuile recouvre une seule et même tuile.");
    }
    //sortie du cadrillage
    for (Coord h : pos) if (h.y>999 || h.y < -99 || h.z < 0) throw CiteException("Placement impossible : Quadrillage trop petit (WIP)");

    // Sauvegarde
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

	//enregistrement de la tuile dans la cité
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
}


void Cite::placerTuileDepart() {
    if (t->getNbHexagones() != 4) throw CiteException("La tuile de départ doit avoir 4 hexagones.");

    //centre en (0,0,0)
    Coord c0 = { 0, 0, 0 };


    //placer autour de (0,0,0)    
	Coord c1 = { -1, 0, 0 }; //ouest
    Coord c2 = { 1, 0, 0 }; //est
    Coord c3 = { 0, 1, 0 }; //nord

	//enregistrement de la tuile dans la cité
    carte[c0] = t->getHexagone(0);
    carte[c1] = t->getHexagone(1);
    carte[c2] = t->getHexagone(2);
    carte[c3] = t->getHexagone(3);
}


vector<Hexagone*> Cite::getAdjacents(Coord c) const {
    vector<Hexagone*> adj;

    // Pour chaque direction autour de la tuile :
    for (const auto& vec : getVecteursVoisins(c.x % 2)) {
        for (int z = 0; z <= getHauteurMax(); ++z) { // on regarde toutes les hauteurs possibles
            Coord voisin = { c.x + vec.x, c.y + vec.y, z };
            auto it = carte.find(voisin);

            if (it != carte.end()) {
                adj.push_back(it->second);
                break; // On a trouvé le voisin visible, on passe à la direction suivante
            }
        }
    }
    return adj;
}

//renvoie la hauteur maximale à laquelle est placée une tuile dans la cité
int Cite::getHauteurMax() const {
    int maxZ = 0;

    for (auto it = begin(); it != end(); ++it) {
		int zActuel = it->first.z; //it->first est une Coord
        if (zActuel > maxZ) {
            maxZ = zActuel;
        }
    }
    return maxZ;
}




