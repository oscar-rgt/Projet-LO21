#ifndef IA_H
#define IA_H

#include "joueur.h"
#include "chantier.h"
#include <vector>

using namespace std;

class IA : public Joueur {
private:
    int difficulte; //1: Hippodamos, 2: Métagénès, 3: Callicratès
    vector<Tuile*> tuilesAcquises;
	TuileDepart* tuileDepart; 

public:
    IA(const string& nom, int diff);
    virtual ~IA();

    virtual int choisirTuile(const Chantier& chantier);

    //ajoute la tuile aux tuiles acquises SANS LA PLACER DANS UNE CITE
    void ajouterTuile(Tuile* t);

    int getDifficulte() const { return difficulte; }

    bool estIA() const override { return true; }

    class Iterator {
        friend class IA;
        vector<Tuile*>::const_iterator current;
        Iterator(vector<Tuile*>::const_iterator it) : current(it) {}
    public:
        Iterator() {}
        Iterator& operator++() { ++current; return *this; }
        bool operator!=(const Iterator& other) const { return current != other.current; }
        bool operator==(const Iterator& other) const { return current == other.current; }
        Tuile* operator*() const { return *current; }
    };

    Iterator begin() const { return Iterator(tuilesAcquises.begin()); }
    Iterator end() const { return Iterator(tuilesAcquises.end()); }
};

#endif
