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
    Tuile* getTuile(unsigned int id_);
    size_t getNbTuiles() const { return tuiles.size(); }
    bool estVide() { return tuiles.empty(); }
    
private:
    friend class Partie;
    ~Pile();
    Pile(unsigned int id_, unsigned int nb = 3);
    unsigned int id;
    vector<Tuile*> tuiles;
};

#endif
