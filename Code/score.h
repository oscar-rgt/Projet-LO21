#ifndef SCORE_H
#define SCORE_H
#include <map>
#include <vector>
#include "hexagone.h"


enum class TypeQuartier {
    Habitation,
    Marche,
    Caserne,
    Temple,
    Jardin
};

// Déclaration anticipée
class Joueur;
class Cite;

class Score {
private:
    Joueur* joueur;
    std::map<TypeQuartier, int> pointsParType;
    int total;

public:
    Score(Joueur* j, int tot = 0);
    void calculerScore();
    int calculerScoreType(TypeQuartier type);
    int getTotal() const { return total; }
    int getScoreType(TypeQuartier type) const;
};

#endif
