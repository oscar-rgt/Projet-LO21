#ifndef PILE_H
#define PILE_H
#include"tuiles.h"

class PileException {
    string info;
public:
    PileException(const string& s) :info(s) {}
    const string& getInfo() const { return info; }
};

class Pile {
public:
    Pile(unsigned int id_, unsigned int nb=3);
    void retirerTuile(unsigned int id_);
    unsigned int getNbTuiles() const { return nbTuiles; }
    unsigned int getId() const { return id; }
    Tuile getTuile(unsigned int id_) const;
    void decalagePrix();
    ~Pile();
private:
    unsigned int id;
    unsigned int nbTuiles;
    Tuile* tuiles;
};

#endif
