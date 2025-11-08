#include "score.h"
#include <iostream>
using namespace std;

Score::Score(Joueur* j, Cité* c)
    : joueur(j), cite(c), total(0)
{
    for (auto type : {TypeQuartier::Habitation, TypeQuartier::Marche,
                      TypeQuartier::Caserne, TypeQuartier::Temple,
                      TypeQuartier::Jardin, TypeQuartier::Place}) {
        pointsParType[type] = 0;
    }
}

int Score::getTotal() const {
    return total;
}

int Score::getScoreType(TypeQuartier type) const {
    auto it = pointsParType.find(type);
    return (it != pointsParType.end()) ? it->second : 0;
}

const Joueur* Score::getJoueur() const {
    return joueur;
}

//score global
void Score::calculerScore() {
    total = 0;
    for (auto type : {TypeQuartier::Habitation, TypeQuartier::Marche,
                      TypeQuartier::Caserne, TypeQuartier::Temple,
                      TypeQuartier::Jardin, TypeQuartier::Place}) {
        int pts = calculerScoreType(type);
        pointsParType[type] = pts;
        total += pts;
    }
    joueur->setPoints(total);
}

//score par type
int Score::calculerScoreType(TypeQuartier type) {
    int score = 0;
    int multiplicateur = 0;
    const vector<Hexagone*>& hexas = cite->getHexagones();

    //calculer les étoiles
    for (auto* h : hexas) {
        if (h->getType() == Type::Place) {
            switch (type) {
                case TypeQuartier::Habitation: multiplicateur += h->getEtoiles(); break;
                case TypeQuartier::Marche: multiplicateur += h->getEtoiles(); break;
                case TypeQuartier::Caserne: multiplicateur += h->getEtoiles(); break;
                case TypeQuartier::Temple: multiplicateur += h->getEtoiles(); break;
                case TypeQuartier::Jardin: multiplicateur += h->getEtoiles(); break;
                case TypeQuartier::Place: multiplicateur += h->getEtoiles(); break;
            }
        }
    }

    if (multiplicateur == 0) return 0;

    //cas par cas selon tuile
    for (auto* h : hexas) {
        switch (type) {
            case TypeQuartier::Habitation:
                if (h->getType() == Type::Habitation) score += 1;
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
                    if (isIsolee) score += 1;
                }
                break;
            case TypeQuartier::Temple:
                if (h->getType() == Type::Temple) {
                    if (cite->getAdjacents(h).size() == 6) score += 2;
                }
                break;
            case TypeQuartier::Jardin:
                if (h->getType() == Type::Jardin) score += 1;
                break;
            case TypeQuartier::Place:
                break;
        }
    }

    score *= multiplicateur;
    return score;
}
