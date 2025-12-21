#pragma once
#include "tuiles.h"
#include "hexagone.h"
#include <vector>
#include <unordered_map>
#include "except.h"
#include "coord.h"

class Joueur;

class CiteException : public AkropolisException {
public:
    CiteException(const string& i) : AkropolisException(i) {}
};

using namespace std;

//structures pour la sauvegarde :
struct InfoHexa {
    int type;
    int etoiles;
    bool estPlace;
};

struct Action {
    int tuileId;
    Coord pos;
    bool inversion;
    int rotation;
    InfoHexa hexas[3];
};

class Cite {
private:
	unordered_map<Coord, Hexagone*> carte; //Espace 3D de pointeurs d'hexagones  / carte[{0, 0, 0}] = hexa0;
    TuileDepart* t;
    vector<Action> historique;

    const bool toucheCite(Coord c) const;

public:
	//itérateur constant pour parcourir la carte
    class ConstIterator {
        friend class Cite;
        unordered_map<Coord, Hexagone*>::const_iterator current;
        ConstIterator(unordered_map<Coord, Hexagone*>::const_iterator it) : current(it) {}
    public:
        ConstIterator() {}
        ConstIterator& operator++() { ++current; return *this; }
        bool operator!=(const ConstIterator& other) const { return current != other.current; }
        bool operator==(const ConstIterator& other) const { return current == other.current; }

        //renvoie la paire {Coord, Hexagone*}
        const pair<const Coord, Hexagone*>& operator*() const { return *current; }
        const pair<const Coord, Hexagone*>* operator->() const { return &(*current); }
    };

    ConstIterator begin() const { return ConstIterator(carte.begin()); }
    ConstIterator end() const { return ConstIterator(carte.end()); }

    Cite() : t(new TuileDepart) {}
    ~Cite() { delete t; }

    void placer(Tuile* t, Coord c, Joueur* j, int rotation = 0);
    void placerTuileDepart();

    bool estLibre(Coord c) const { return (carte.find(c) == carte.end()); }

    vector<Hexagone*> getAdjacents(Coord c) const;
    vector<Coord> getVecteursVoisins(bool isXOdd) const;

    const vector<Action>& getHistorique() const { return historique; }

    int getHauteurMax() const;
};
