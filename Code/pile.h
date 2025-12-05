#ifndef PILE_H
#define PILE_H
#include "tuiles.h"
#include <vector>

class Partie;

class PileException {
    string info;
public:
    PileException(const string& s) :info(s) {}
    const string& getInfo() const { return info; }
};

class Pile {
public:
    unsigned int getId() const { return id; }
    size_t getNbTuiles() const { return tuiles.size(); }
    bool estVide() const { return tuiles.empty(); } // const ajouté

    // SUPPRIMÉ : Tuile* getTuile(unsigned int id_);

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