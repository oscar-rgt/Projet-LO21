#ifndef PILE_H
#define PILE_H
#include "tuiles.h"
#include <vector>


class Partie;


class Pile {
public:
    unsigned int getId() const { return id; }
    int getNbTuiles() const { return static_cast<int>(tuiles.size()); }
    bool estVide() const { return tuiles.empty(); }

    // =========================================================
    // ITÉRATEUR POUR LA PILE
    // =========================================================
    class Iterator {
        friend class Pile;
        std::vector<Tuile*>::const_iterator current;
        Iterator(std::vector<Tuile*>::const_iterator it) : current(it) {}
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
    friend class Partie;
    ~Pile();
    Pile(unsigned int id_, unsigned int nb = 3);
    unsigned int id;
    vector<Tuile*> tuiles;
};

#endif