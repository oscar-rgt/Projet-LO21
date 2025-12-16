#ifndef PILE_H
#define PILE_H
#include "tuiles.h"
#include <vector>


class Partie;
class SaveManager;


class Pile {
public:
    static void detruire(Pile* p) { delete p; }
    static Pile* creer(unsigned int id) { 
        return new Pile(id); 
    }
    unsigned int getId() const { return id; }
    int getNbTuiles() const { return static_cast<int>(tuiles.size()); }
    bool estVide() const { return tuiles.empty(); }
    const std::vector<Tuile*>& getTuiles() const { return tuiles; }
    void vider() { tuiles.clear(); }
    void ajouterTuile(Tuile* t) { tuiles.push_back(t); }

    // =========================================================
    // IT�RATEUR POUR LA PILE
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
    friend class SaveManager;
    ~Pile();
    Pile(unsigned int id_, unsigned int nb = 3);
    unsigned int id;
    std::vector<Tuile*> tuiles;
};

#endif