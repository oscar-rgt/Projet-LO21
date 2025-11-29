#ifndef JOUEUR_H
#define JOUEUR_H

#include <string>
#include "score.h"

using namespace std;

class Joueur {
private:
    string nom;
    int points;
    int pierres;
    Cite* cite;

public:
    Joueur(const string& n);
    const string& getNom() const;

    int getPoints() const;
    void setPoints(int p);
    void ajouterPoints(int delta);

    int getPierres() const;
    void ajouterPierres(int n = 1);
    bool utiliserPierres(int n = 1);
    void ajouterPierresRecouvrement(int nbRecouverts);

    Cite* getCite() const;   // CORRIGÉ : bon nom
    void setCite(Cite* c);
};

#endif
