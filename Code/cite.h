#pragma once
#include "tuiles.h"
#include "hexagone.h"
#include <vector>
#include <unordered_map>

class Joueur;

using namespace std;
struct InfoHexa {
    int type;
    int etoiles;
    bool estPlace;
};

struct Action {
    int tuileId;
    int x, y, z;
    bool inversion;
    InfoHexa hexas[3];
};

struct Coord { //Tuple de coordonnées 
    int x, y, z;
    Coord sud() { return { x,y - 1,z }; }
    Coord cote(bool inversion);
    bool operator==(const Coord& other) const noexcept { return x == other.x && y == other.y && z == other.z; }
};
struct CoordHash {
    size_t operator()(const Coord& c) const noexcept {
        size_t hx = hash<int>()(c.x);
        size_t hy = hash<int>()(c.y);
        size_t hz = hash<int>()(c.z);
		// combine les trois hash
        return hx ^ (hy << 1) ^ (hz << 2);
    }
};

class CiteException {
private:
    string infos;
public:
    const string getInfos() const { return infos; }
    CiteException(string i) : infos(i) {}
};

class Cite {
private:
	unordered_map<Coord, Hexagone*, CoordHash> carte; // Espace 3D de pointeurs d'hexagones  / carte[{0, 0, 0}] = tuile0;
    const bool toucheCite(Coord c);
    string quadrillage;
    TuileDepart* t;
    vector<Action> historique;

public:
    // ==========================================
    // ITERATOR
    // ==========================================
    class ConstIterator {
        friend class Cite;
        unordered_map<Coord, Hexagone*, CoordHash>::const_iterator current;
        ConstIterator(unordered_map<Coord, Hexagone*, CoordHash>::const_iterator it) : current(it) {}
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

    Cite() : quadrillage(R"(
          _____         _____         _____         _____         _____         _____         _____          
         /     \       /     \       /     \       /     \       /     \       /     \       /     \          
   _____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____    
3 /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \  
 /       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \ 
 \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       / 
  \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/  
2 /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \  
 /       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \ 
 \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       / 
  \_____/       \_____/       \_____/       \_____/   X0  \_____/       \_____/       \_____/       \_____/  
1 /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \  
 /       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \ 
 \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       / 
  \_____/       \_____/       \_____/       \_____/  1H0  \_____/       \_____/       \_____/       \_____/  
0 /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \  
 /       \_____/       \_____/       \_____/   X0  \_____/   X0  \_____/       \_____/       \_____/       \ 
 \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       / 
  \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/  
-1/     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \  
 /       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \ 
 \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       / 
  \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/  
-2/     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \  
 /       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \ 
 \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       / 
  \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/  
     -7     -6     -5    -4     -3     -2     -1      0      1      2      3      4      5      6      7     
)"), t(new TuileDepart) {
    }
    ~Cite() { delete t; }
    void placer(Tuile* t, Coord c, Joueur* j);
    void placerTuileDepart();
    const bool estLibre(Coord c) const { return (carte.find(c) == carte.end()); }
    const bool estRecouvert(Coord c) const { return (!estLibre({ c.x, c.y, c.z + 1 })); }
    void afficher() const;
    void remplirQuadrillage(Coord c, Tuile& t);
    vector<Hexagone*> getAdjacents(Coord c) const;
    vector<Coord> getVecteursVoisins() const;
    void agrandirQ(char dir);
    const vector<Action>& getHistorique() const { return historique; }
};