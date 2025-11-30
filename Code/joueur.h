
#ifndef JOUEUR_H
#define JOUEUR_H

#include <string>

using namespace std;

class Cite;

class Joueur {
private:
    string nom;
    int points;
    int pierres;
    Cite* cite;

public:
    Cite* getCite() const { return cite; }
    void setCite(Cite* c) { cite = c; }
    Joueur(const string& n);
    const string& getNom() const;
    int getPoints() const;
    int getPierres() const;
    void setPoints(int p);
    void ajouterPoints(int delta);
    void ajouterPierres(int n = 1);
    bool utiliserPierres(int n = 1);
    void ajouterPierresRecouvrement(int nbRecouverts);
};

#endif

