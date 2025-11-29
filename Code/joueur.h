#ifndef JOUEUR_H
#define JOUEUR_H

#include <string>

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
    int getPierres() const;
    Cite* getcite() const;
    void setCite(Cite* c);
    void setPoints(int p);
    void ajouterPoints(int delta);
    void ajouterPierres(int n = 1);
    bool utiliserPierres(int n = 1);
    void ajouterPierresRecouvrement(int nbRecouverts);
};

#endif
