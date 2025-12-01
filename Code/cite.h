#pragma once
#include "tuiles.h"
#include "hexagone.h"
#include "score.h"
#include <vector>
#include <unordered_map>

using namespace std;

class CiteException {
private:
	string infos;
public:
	const string getInfos() const { return infos; }
	CiteException(string i) : infos(i) {}
};

class Cite {
private :
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
	unordered_map<Coord, Hexagone*, CoordHash> carte; // Espace 3D de pointeurs d'hexagones 
	//  carte[{0, 0, 0}] = tuile0;
	const bool toucheCite(Coord c);
	string quadrillage;
	TuileDepart *t;
public :
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
)"), t(new TuileDepart) {}
	~Cite() { delete t; }
	void placer(Tuile* t, Coord c);
    void placerTuileDepart();
	const bool estLibre(Coord c) const { return (carte.find(c) == carte.end()); }
	const bool estRecouvert(Coord c) const { return (!estLibre({ c.x, c.y, c.z + 1 })); }
	void afficher() const;
	void remplirQuadrillage(Coord c, Tuile& t);
	vector<Hexagone*> getAdjacents(Coord c);
	void afficherMap() const;
	const unordered_map<Coord, Hexagone*, CoordHash>& getCarte() const { return carte; }
	vector<Cite::Coord> getVecteursVoisins();
	void agrandirQ(char dir);
};
