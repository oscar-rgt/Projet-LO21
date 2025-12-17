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
	unordered_map<Coord, Hexagone*> carte; // Espace 3D de pointeurs d'hexagones  / carte[{0, 0, 0}] = hexa0;
    const bool toucheCite(Coord c) const;
    TuileDepart* t;
    vector<Action> historique;

public:
    // ==========================================
    // ITERATOR CONST 
    // ==========================================
    class ConstIterator {
        friend class Cite;
        unordered_map<Coord, Hexagone*>::const_iterator current;
        ConstIterator(unordered_map<Coord, Hexagone*>::const_iterator it) : current(it) {}
    public:
        ConstIterator() {}
        ConstIterator& operator++() { ++current; return *this; }
        bool operator!=(const ConstIterator& other) const { return current != other.current; }
        bool operator==(const ConstIterator& other) const { return current == other.current; }

        // Renvoie la paire {Coord, Hexagone*}
        const pair<const Coord, Hexagone*>& operator*() const { return *current; }
        const pair<const Coord, Hexagone*>* operator->() const { return &(*current); }
    };

    ConstIterator begin() const { return ConstIterator(carte.begin()); }
    ConstIterator end() const { return ConstIterator(carte.end()); }

    const Hexagone* getHexagone(Coord c) const;

    Cite() : t(new TuileDepart) {}
    ~Cite() { delete t; }
    void placer(Tuile* t, Coord c, Joueur* j, int rotation = 0);
    void placerTuileDepart();
    bool estLibre(Coord c) const { return (carte.find(c) == carte.end()); }
    bool estRecouvert(Coord c) const { return (!estLibre({ c.x, c.y, c.z + 1 })); }
    vector<Hexagone*> getAdjacents(Coord c) const;
    vector<Coord> getVecteursVoisins(bool isXOdd) const;
    const vector<Action>& getHistorique() const { return historique; }
    int getHauteurMax() const;
};
