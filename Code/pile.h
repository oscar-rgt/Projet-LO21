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
<<<<<<< Updated upstream
=======
    //Pour sauvegarder
    void vider() { tuiles.clear(); } // INDISPENSABLE
    void ajouterTuile(Tuile* t) { tuiles.push_back(t); }
    const vector<Tuile*>& getTuiles() const { return tuiles; }
>>>>>>> Stashed changes

private:
    friend class Partie;
    ~Pile();
    Pile(unsigned int id_, unsigned int nb = 3);
    unsigned int id;
    vector<Tuile*> tuiles;
};

#endif
