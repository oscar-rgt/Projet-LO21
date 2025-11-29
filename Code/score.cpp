#include "score.h"
#include <iostream>
using namespace std;

Score::Score(Joueur* j)
    : joueur(j), cite(j->getCite()), total(0)
{
    for (auto t : {TypeQuartier::Habitation, TypeQuartier::Marche,
                   TypeQuartier::Caserne, TypeQuartier::Temple,
                   TypeQuartier::Jardin}) {
        pointsParType[t] = 0;
    }
}

int Score::getTotal() const {
    return total;
}

int Score::getScoreType(TypeQuartier type) const {
    auto it = pointsParType.find(type);
    return (it != pointsParType.end()) ? it->second : 0;
}

Joueur* Score::getJoueur() const {  
    return joueur;
}

//score total
void Score::calculerScore() {
    total = 0;

    for (auto type : {TypeQuartier::Habitation, TypeQuartier::Marche,
                      TypeQuartier::Caserne, TypeQuartier::Temple,
                      TypeQuartier::Jardin}) {

        int pts = calculerScoreType(type);
        pointsParType[type] = pts;
        total += pts;
    }

    joueur->setPoints(total);
}

//score selon type
int Score::calculerScoreType(TypeQuartier type) {
    int score = 0;
    int multiplicateur = 0;

    // calcul des étoiles des Places
    for (auto it = cite->carte.begin(); it != cite->carte.end(); ++it) {
        Hexagone* h = it->second;
        if (h->getType() == Type::Place)
            multiplicateur += h->getEtoiles();
    }

    if (multiplicateur == 0) return 0;

    //deuxième boucle : calcul du score selon le type
    for (auto it = cite->carte.begin(); it != cite->carte.end(); ++it) {
        Hexagone* h = it->second;

        switch (type) {
        case TypeQuartier::Habitation:
            if (h->getType() == Type::Habitation)
                score += 1;
            break;

        case TypeQuartier::Marche:
            if (h->getType() == Type::Marche) {
                int ptsAdj = 0;
                for (auto* v : cite->getAdjacents(h)) {
                    if (v->getType() != Type::Marche && v->getType() != Type::Place)
                        ptsAdj++;
                }
                score += ptsAdj;
            }
            break;

        case TypeQuartier::Caserne:
            if (h->getType() == Type::Caserne) {
                bool isIsolee = true;
                for (auto* v : cite->getAdjacents(h)) {
                    if (v->getType() == Type::Caserne) {
                        isIsolee = false;
                        break;
                    }
                }
                if (isIsolee)
                    score += 1;
            }
            break;

        case TypeQuartier::Temple:
            if (h->getType() == Type::Temple) {
                if (cite->getAdjacents(h).size() == 6)
                    score += 2;
            }
            break;

        case TypeQuartier::Jardin:
            if (h->getType() == Type::Jardin)
                score += 1;
            break;
        }
    }

    score *= multiplicateur;
    return score;
}
