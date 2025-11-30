#include <iostream>
#include "placement.h"
#include "score.h"
#include "tuiles.h"
using namespace std;

const bool Cite::toucheCite(Coord c) const { 
    """L'hexagone aux coos c touche-t-il la cité ?""";
    return (!estLibre({ c.x - 1, c.y - 1, c.z }) || !estLibre({ c.x, c.y - 1, c.z }) || !estLibre({ c.x + 1, c.y - 1, c.z })
        || !estLibre({ c.x - 1, c.y, c.z }) || !estLibre({ c.x, c.y + 1, c.z }) || !estLibre({ c.x + 1, c.y, c.z }));
}

void Cite::placer(Tuile* t, Coord c){ // Il faut tourner la tuile avant de la placer
    Coord c2 = c.sud(), c3 = c.cote(t->getInversion()); 
    if (!estLibre(c) || !estLibre(c2) || !estLibre(c3))
        throw CiteException("Placement impossible : Une des cases n'est pas libre.");

	
    if (c.z == 0) if (!toucheCite(c) && !toucheCite(c2) && !toucheCite(c3)) throw CiteException("Placement impossible : L'emplacement ne touche pas la cité.");
    else {
    // Vérifier que chaque hexagone de la tuile est soutenu par un hexagone en dessous
    Coord dessous_c = {c.x, c.y, c.z - 1};
    Coord dessous_c2 = {c2.x, c2.y, c2.z - 1};
    Coord dessous_c3 = {c3.x, c3.y, c3.z - 1};
    
    if (estLibre(dessous_c) || estLibre(dessous_c2) || estLibre(dessous_c3)) throw CiteException("Placement impossible : Les cases inférieures sont vides.");
        Tuile* tuile_dessous_c = carte[dessous_c]->getTuile();
        Tuile* tuile_dessous_c2 = carte[dessous_c2]->getTuile();
        Tuile* tuile_dessous_c3 = carte[dessous_c3]->getTuile();
    
    bool deuxTuilesDifferentes = (tuile_dessous_c != tuile_dessous_c2) || 
                                  (tuile_dessous_c != tuile_dessous_c3) || 
                                  (tuile_dessous_c2!= tuile_dessous_c3);
    
    if (!deuxTuilesDifferentes) 
        throw CiteException("Placement impossible : Les cases inférieures sont dans la même tuile.");
    }
    carte[c] = t->getHexagone(0);
    carte[c2] = t->getHexagone(1);
    carte[c3] = t->getHexagone(2);
    remplirQuadrillage(c, *t);
}

void Cite::placerTuileDepart() {
    if (t->getNbHexagones() != 4) throw CiteException("La tuile de départ doit avoir 4 hexagones.");
    
    // Centre (Habitation) en (0,0,0)
    Coord c0 = { 0, 0, 0 };
    
    
    // placer autour de (0,0,0)    
    Coord c1 = { 0, -1, 0 }; //NORD
    Coord c2 = { -1, 1, 0 };
    Coord c3 = { 1, 1, 0 };

    carte[c0] = t->getHexagone(0);
    carte[c1] = t->getHexagone(1);
    carte[c2] = t->getHexagone(2);
    carte[c3] = t->getHexagone(3);
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

Cite::Coord Cite::Coord::cote(bool inversion) {
    if (inversion) return { x - 1, y,z };
    else return { x + 1, y,z };
}

vector<Hexagone*> Cite::getAdjacents(Coord c){
    if (!toucheCite(c)) throw CiteException("Cet hexagone ne touche pas la cité.");
    vector<Coord> coo = { {c.x - 1, c.y - 1, c.z},  { c.x, c.y - 1, c.z }, { c.x + 1, c.y - 1, c.z },
        { c.x - 1, c.y, c.z }, { c.x, c.y + 1, c.z }, { c.x + 1, c.y, c.z } };
    vector<Hexagone*> ret;
    for (Coord c : coo) {
        if (!estLibre(c)) ret.push_back(carte[c]);
    }
    return ret;
}


void Cite::afficherMap() const {
    // On parcourt toute la map
    for (const auto& paire : carte) {
        // paire.first contient la Coordonnée (la clé)
        cout << "(" << paire.first.x << ", "
            << paire.first.y << ", "
            << paire.first.z << ")" << endl;
    }
}