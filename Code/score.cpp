#include "score.h"
#include <iostream>

// constructeur
Score::Score(Joueur* j, Plateau* p)
    : joueur(j), plateau(p), total(0)
{
    for (auto type : {TypeQuartier::Habitation, TypeQuartier::Marche,
                      TypeQuartier::Caserne,   TypeQuartier::Temple,
                      TypeQuartier::Jardin})
    {
        pointsParType[type] = 0;
    }
}


// --- GETTERS ---
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


// --- CALCUL GLOBAL ---
void Score::calculerScore() {
    total = 0;

    for (auto type : {TypeQuartier::Habitation, TypeQuartier::Marche,
                      TypeQuartier::Caserne,   TypeQuartier::Temple,
                      TypeQuartier::Jardin})
    {
        int pts = calculerScoreType(type);
        pointsParType[type] = pts;
        total += pts;
    }

    joueur->setPoints(total);
}


// --- CALCUL PAR TYPE ---
int Score::calculerScoreType(TypeQuartier type) {
    int score = 0;
    int multiplicateur = 0;

    // récupère tous les hexagones depuis le plateau
    const std::vector<Hexagone*>& hexas = plateau->getHexagones();

    // calcul du multiplicateur via les étoiles des Places
    for (auto* h : hexas) {
        if (h->getType() == Type::Place && h->getTypeAssocie() == type) {
            multiplicateur += h->getEtoiles();
        }
    }

    if (multiplicateur == 0) return 0;

    // scoring : cas par cas
    for (auto* h : hexas) {

        switch (type) {

        case TypeQuartier::Habitation:
            if (h->getType() == Type::Habitation)
                score += 1;
            break;

        case TypeQuartier::Marche:
            if (h->getType() == Type::Marche) {
                int ptsAdj = 0;
                for (auto* v : plateau->getAdjacents(h)) {
                    if (v->getType() != Type::Marche && v->getType() != Type::Place)
                        ptsAdj++;
                }
                score += ptsAdj;
            }
            break;

        case TypeQuartier::Caserne:
            if (h->getType() == Type::Caserne) {
                bool isIsolee = true;
                for (auto* v : plateau->getAdjacents(h)) {
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
                if (plateau->getAdjacents(h).size() == 6)
                    score += 2;
            }
            break;

        case TypeQuartier::Jardin:
            if (h->getType() == Type::Jardin)
                score += 1;
            break;
        }
    }

    // appliquer multiplicateur
    return score * multiplicateur;
}
