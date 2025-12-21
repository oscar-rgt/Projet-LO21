#ifndef PILE_H
#define PILE_H
#include "tuiles.h"
#include <vector>

using namespace std;

// Forward declarations pour éviter les inclusions circulaires
class Partie;
class SaveManager;


class Pile {
public:
    unsigned int getId() const { return id; }
    int getNbTuiles() const { return static_cast<int>(tuiles.size()); }
    void vider() { tuiles.clear(); }
    void ajouterTuile(Tuile* t) { tuiles.push_back(t); }

    // =========================================================
    // ITERATEUR POUR LA PILE
    // =========================================================
    class Iterator {
        friend class Pile;
        vector<Tuile*>::const_iterator current;
        Iterator(vector<Tuile*>::const_iterator it) : current(it) {}
    public:
        Iterator() {}
        Iterator& operator++() { ++current; return *this; }
        bool operator!=(const Iterator& other) const { return current != other.current; }
        bool operator==(const Iterator& other) const { return current == other.current; }
        Tuile* operator*() const { return *current; }
        Tuile* operator->() const { return *current; }
    };

    Iterator begin() const { return Iterator(tuiles.begin()); }
    Iterator end() const { return Iterator(tuiles.end()); }

private:
	friend class Partie; // important pour créer les piles de la partie
	friend class SaveManager; // important pour charger les piles
    ~Pile();
    Pile(unsigned int id_, unsigned int nb = 3);
	unsigned int id; //ID unique de la pile
	vector<Tuile*> tuiles; //Conteneur des tuiles dans la pile
};

#endif