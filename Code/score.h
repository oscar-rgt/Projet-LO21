#ifndef SCORE_H
#define SCORE_H

#include <map>
#include "joueur.h"
#include "cite.h"
#include "hexagone.h"

enum class TypeQuartier {
    Habitation,
    Marche,
    Caserne,
    Temple,
    Jardin
};

class Score {
private:
    Joueur* joueur;             // Joueur concerné
    Cité* cite;                 // Référence à la cité du joueur
    std::map<TypeQuartier, int> pointsParType;  // Score détaillé par type de quartier
    int total;                  // Score total final

public:
    Score(Joueur* j, Cité* c);
    void calculerScore();
    int calculerScoreType(TypeQuartier type);// Calcul du score d’un seul type de quartier
    int getTotal() const;
    int getScoreType(TypeQuartier type) const;
    const Joueur* getJoueur() const;
};

#endif
