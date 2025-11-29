#ifndef PARTIE_H
#define PARTIE_H

#include <array>
#include <iostream>
#include "joueur.h"
#include "pile.h"

using namespace std;
 

class Partie {
public:
    enum Var { V1, V2, V3, V4, V5 };
    enum tuileCite { standard, augmente };

    static Partie& getInstance(int id_, Pile* pile, array<bool, 5> variantes);

    Partie(const Partie&) = delete;
    Partie& operator=(const Partie&) = delete;

    bool ajouterJoueur(Joueur* j);
    int getId() const;
    int getNbJoueurs() const;
    Joueur* getJoueur(int index) const;
    Pile* getPile() const;
    void jouerTours();
    bool aVariante(Var v) const { return variantes[v]; }

    bool menu();
    void initPartie(bool reprise);
    void bouclePartieMulti();

private:
    Partie(int id_);

    array<bool, 5> variantes;
    int id;
    array<Joueur*, 4> joueurs;
    int nbJoueurs;
    Pile* piles;
    unsigned int niveauIllustreConstructeur;
    tuileCite modeTuileCite;
    
};

#endif
