#ifndef JOUEUR_H
#define JOUEUR_H

#include <string>
#include "cite.h"
#include "score.h"

using namespace std;

class Joueur {
private:
    string nom;
    int pierres;
    Cite* cite;
    Score* score;

public:
    Joueur(const string& n);

    ~Joueur();

    const string& getNom() const;

    Cite* getCite() const;
    void setCite(Cite* c);

    Score* getScore() const;
    void calculerScore();

    int getPierres() const;
    void ajouterPierres(int n = 1);
    bool utiliserPierres(int n = 1);
    void ajouterPierresRecouvrement(int nbRecouverts);
};

#endif
