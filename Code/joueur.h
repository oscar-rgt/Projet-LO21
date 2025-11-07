#ifndef JOUEUR_H
#define JOUEUR_H

#include <string>

class Joueur {
private:
    std::string nom;
    int points;
    int pierres;

public:
    Joueur(const std::string& n);
    const std::string& getNom() const;
    int getPoints() const;
    int getPierres() const;
    void setPoints(int p);
    void ajouterPoints(int delta);
    void ajouterPierres(int n = 1);
    bool utiliserPierres(int n = 1);
    void ajouterPierresRecouvrement(int nbRecouverts);
};

#endif
