#ifndef SCORE_H
#define SCORE_H

#include <map>
#include <vector>
#include "joueur.h"
#include "hexagone.h"
#include "placement.h"

enum class TypeQuartier {
    Habitation,
    Marche,
    Caserne,
    Temple,
    Jardin
};

class Score {
private:
    const Joueur* joueur;
    const Cite* cite;
    std::map<TypeQuartier, int> pointsParType;
    int total;

public:
    Score(const Joueur* j);
    void calculerScore();
    int calculerScoreType(TypeQuartier type) const;
    int getTotal() const;
    int getScoreType(TypeQuartier type) const;
    const Joueur* getJoueur() const;
};

#endif
