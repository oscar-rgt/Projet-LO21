#ifndef PARTIE_H
#define PARTIE_H

#include <array>
#include <string>
#include <iostream>
#include "joueur.h"

class Partie {
public:
    enum Mode { solo, multi };

private:
    int id;
    Mode mode;
    std::array<Joueur*, 4> joueurs;
    int nbJoueurs;

public:
    Partie(int id_, Mode m);
    bool ajouterJoueur(Joueur* j);
    int getId() const;
    Mode getMode() const;
    int getNbJoueurs() const;
    Joueur* getJoueur(int index) const;
    void jouerTours(int maxTours);
};

#endif
