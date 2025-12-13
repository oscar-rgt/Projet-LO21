#ifndef PILE_H
#define PILE_H
#include"tuiles.h"
#include<vector>

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
<<<<<<< Updated upstream
    Tuile* getTuile(unsigned int id_);
    size_t getNbTuiles() const { return tuiles.size(); }
    bool estVide() { return tuiles.empty(); }
    //Pour sauvegarder
    void vider() { tuiles.clear(); } // INDISPENSABLE
    void ajouterTuile(Tuile* t) { tuiles.push_back(t); }
    const vector<Tuile*>& getTuiles() const { return tuiles; }
=======
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
>>>>>>> Stashed changes

private:
    friend class Partie;
    ~Pile();
    Pile(unsigned int id_, unsigned int nb = 3);
    unsigned int id;
    vector<Tuile*> tuiles;
};

#endif
