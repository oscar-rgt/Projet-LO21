#ifndef PILE_H
#define PILE_H
#include"tuiles.h"
#include<vector>

class PileException {
    string info;
public:
    PileException(const string& s) :info(s) {}
    const string& getInfo() const { return info; }
};

class Pile {
public:
    Pile(unsigned int id_, unsigned int nb=3);
    unsigned int getId() const { return id; }
    Tuile* getTuile(unsigned int id_);
    size_t getNbTuiles() const { return tuiles.size(); }
    void decalagePrix();
    bool estVide() { return tuiles.empty(); }
private:
    unsigned int id;
    vector<Tuile> tuiles;
};

#endif
