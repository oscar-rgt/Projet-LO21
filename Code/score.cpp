#include "score.h"
#include "partie.h"
#include "joueur.h"
#include "cite.h"
#include <stdexcept>

Score::Score(Joueur* j)
    : joueur(j), total(0)
{
    for (auto t : {TypeQuartier::Habitation, TypeQuartier::Marche,
                   TypeQuartier::Caserne, TypeQuartier::Temple,
                   TypeQuartier::Jardin}) {
        pointsParType[t] = 0;
    }
}



int Score::getScoreType(TypeQuartier type) const {
    auto it = pointsParType.find(type);
    return (it != pointsParType.end()) ? it->second : 0;
}


void Score::calculerScore() {
    if (!joueur->getCite()) {
        total = 0;
        return;
    }

    total = 0;
    for (auto type : {TypeQuartier::Habitation, TypeQuartier::Marche,
                      TypeQuartier::Caserne, TypeQuartier::Temple,
                      TypeQuartier::Jardin}) {
        int pts = calculerScoreType(type);
        pointsParType[type] = pts;
        total += pts;
    }
}

int Score::calculerScoreType(TypeQuartier type) {
    if (!joueur->getCite()) return 0;

    int score = 0;
    int multiplicateur = 0;

    for (const auto& it : joueur->getCite()->getCarte()) {
        Hexagone* h = it.second;
        if (!h) continue;
        if (h->getType() == Type::Place) multiplicateur += h->getEtoiles();
    }

    if (multiplicateur == 0) return 0;

    for (const auto& it : joueur->getCite()->getCarte()) {
        Hexagone* h = it.second;
        if (!h) continue;

        switch (type) {
        case TypeQuartier::Habitation:
            if (h->getType() == Type::Habitation) score += 1;
            break;
        case TypeQuartier::Marche:
            if (h->getType() == Type::Marche) {
                int ptsAdj = 0;
                try {
                    auto adjacents = joueur->getCite()->getAdjacents(it.first);
                    for (Hexagone* v : adjacents) {
                        if (!v) continue;
                        if (v->getType() != Type::Marche && v->getType() != Type::Place) ptsAdj++;
                    }
                } catch (const CiteException&) { continue; }
                score += ptsAdj;
            }
            break;
        case TypeQuartier::Caserne:
            if (h->getType() == Type::Caserne) {
                bool isIsolee = true;
                try {
                    auto adjacents = joueur->getCite()->getAdjacents(it.first);
                    for (Hexagone* v : adjacents) {
                        if (!v) continue;
                        if (v->getType() == Type::Caserne) { isIsolee = false; break; }
                    }
                } catch (const CiteException&) { continue; }
                if (isIsolee) score += 1;
            }
            break;
        case TypeQuartier::Temple:
            if (h->getType() == Type::Temple) {
                try {
                    auto adjacents = joueur->getCite()->getAdjacents(it.first);
                    if (adjacents.size() == 6) score += 2;
                } catch (const CiteException&) { continue; }
            }
            break;
        case TypeQuartier::Jardin:
            if (h->getType() == Type::Jardin) score += 1;
            break;
        }
    }

    score *= multiplicateur;

    //VARIANTES CEST ICI ------------------------------------------------
    const auto& vars = Partie::getInstance().getVariantes();
    switch (type) {
    case TypeQuartier::Habitation:
        if (vars[0] && score >= 10) score *= 2;
        break;
    case TypeQuartier::Marche:
        if (vars[1]) {
            for (const auto& it : joueur->getCite()->getCarte()) {
                Hexagone* h = it.second;
                if (!h) continue;
                if (h->getType() == Type::Marche) {
                    try {
                        auto adj = joueur->getCite()->getAdjacents(it.first);
                        for (Hexagone* v : adj) {
                            if (v && v->getType() == Type::Place) { score *= 2; break; }
                        }
                    } catch (const CiteException&) { continue; }
                }
            }
        }
        break;
    case TypeQuartier::Caserne:
        if (vars[2]) {
            for (const auto& it : joueur->getCite()->getCarte()) {
                Hexagone* h = it.second;
                if (!h) continue;
                if (h->getType() == Type::Caserne) {
                    try {
                        auto adj = joueur->getCite()->getAdjacents(it.first);
                        int vides = 0;
                        for (Hexagone* v : adj) if (!v) vides++;
                        if (vides == 3 || vides == 4) { score *= 2; break; }
                    } catch (const CiteException&) { continue; }
                }
            }
        }
        break;
    case TypeQuartier::Temple:
        if (vars[3]) {
            for (const auto& it : joueur->getCite()->getCarte()) {
                Hexagone* h = it.second;
                if (!h) continue;
                if (h->getType() == Type::Temple && it.first.z > 0) { score *= 2; break; }
            }
        }
        break;
    case TypeQuartier::Jardin:
        if (vars[4]) {
            for (const auto& it : joueur->getCite()->getCarte()) {
                Hexagone* h = it.second;
                if (!h) continue;
                if (h->getType() == Type::Jardin) {
                    try {
                        auto adj = joueur->getCite()->getAdjacents(it.first);
                        bool lac = true;
                        for (Hexagone* v : adj) if (v) { lac = false; break; }
                        if (lac) { score *= 2; break; }
                    } catch (const CiteException&) { continue; }
                }
            }
        }
        break;
    }

    return score;
}
