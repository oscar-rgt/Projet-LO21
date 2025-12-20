#ifndef CHANTIER_H
#define CHANTIER_H

#include <vector>
#include "tuiles.h"
#include "pile.h"

class Chantier {
public:
    
    void ajouterPile(Pile& p);
    int getNbTuiles() const { return static_cast<int>(tuiles.size()); }
    void retirerTuile(Tuile* t); 
    bool estVide() const;
    void vider() { tuiles.clear(); } 
    void ajouterTuileSpecifique(Tuile* t);

    class Iterator {
        friend class Chantier;
        std::vector<Tuile*>::const_iterator current;
        Iterator(std::vector<Tuile*>::const_iterator it) : current(it) {}
    public:
        Iterator() {}
        Iterator& operator++() { ++current; return *this; }
        bool operator!=(const Iterator& other) const { return current != other.current; }
        bool operator==(const Iterator& other) const { return current == other.current; }
        Tuile* operator*() const { return *current; }
    };

    Iterator begin() const { return Iterator(tuiles.begin()); }
    Iterator end() const { return Iterator(tuiles.end()); }

private:
    friend class Partie;
    Chantier();
    ~Chantier();
    std::vector<Tuile*> tuiles;
};

#endif