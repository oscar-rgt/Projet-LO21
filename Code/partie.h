#ifndef PARTIE_H
#define PARTIE_H

#include <array>
#include <iostream>
#include "joueur.h"
#include "pile.h"

class Partie {
public:
    enum Mode { solo, multi };

private:
    int id;
    Mode mode;
    std::array<Joueur*, 4> joueurs;
    int nbJoueurs;
    Pile* pileTuiles;

public:
    Partie(int id_, Mode m, Pile* pile);
    bool ajouterJoueur(Joueur* j);
    int getId() const;
    Mode getMode() const;
    int getNbJoueurs() const;
    Joueur* getJoueur(int index) const;
    Pile* getPile() const;
    void jouerTours();
};

#endif
