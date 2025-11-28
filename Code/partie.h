#ifndef PARTIE_H
#define PARTIE_H

#include <array>
#include <iostream>
#include "joueur.h"
#include "pile.h"

using namespace std;

typedef enum {V1, V2, V3, V4, V5} Var;

class Partie {
public:
    enum Mode { solo, multi };

    static Partie& getInstance(int id_, Mode m, Pile* pile, array<bool, 5> variantes);

    Partie(const Partie&) = delete;
    Partie& operator=(const Partie&) = delete;

    bool ajouterJoueur(Joueur* j);
    int getId() const;
    Mode getMode() const;
    int getNbJoueurs() const;
    Joueur* getJoueur(int index) const;
    Pile* getPile() const;
    void jouerTours();
    bool aVariante(Var v) const { return variantes[v]; }

private:
    Partie(int id_, Mode m, Pile* pile, array<bool,5> variantes_);
    array<bool, 5> variantes;
    int id;
    Mode mode;
    array<Joueur*, 4> joueurs;
    int nbJoueurs;
    Pile* pileTuiles;
};

#endif
