#ifndef JOUEUR_H
#define JOUEUR_H

#include <string>
#include "score.h"
#include "tuiles.h"

class Cite;

using namespace std;

class Joueur {
private:
    string nom;
    int pierres;
    int points;
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

    void setPoints(int p) { points = p; }
    int getPoints() const { return points; }

};

#endif
