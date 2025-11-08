#ifndef SCORE_H
#define SCORE_H

#include <map>
#include "joueur.h"
#include "hexagone.h"
#include "plateau.h"

enum class TypeQuartier {
    Habitation,
    Marche,
    Caserne,
    Temple,
    Jardin
};

class Score {
private:
    Joueur* joueur;
    Plateau* plateau;
    std::map<TypeQuartier,int> pointsParType;
    int total;

public:
    Score(Joueur* j, Plateau* p);
    void calculerScore();
    int  calculerScoreType(TypeQuartier type);
    int  getTotal() const;
    int  getScoreType(TypeQuartier type) const;
    const Joueur* getJoueur() const;
};

#endif
