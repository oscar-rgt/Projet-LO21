#include "score.h"
#include <stdexcept>

Score::Score(const Joueur* j)
    : joueur(j), cite(j ? j->getCite() : nullptr), total(0)
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

const Joueur* Score::getJoueur() const {
    return joueur;
}

void Score::calculerScore() {
    if (!cite) {
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

    if (joueur) {
        const_cast<Joueur*>(joueur)->setPoints(total);
    }
}

int Score::calculerScoreType(TypeQuartier type) const {
    if (!cite) return 0;

    int score = 0;
    int multiplicateur = 0;

    // Calcul du multiplicateur (places)
    for (const auto& it : cite->getCarte()) {
        const Hexagone* h = it.second;
        if (!h) continue;
        if (h->getType() == Type::Place) {
            multiplicateur += h->getEtoiles();
        }
    }

    if (multiplicateur == 0) return 0;

    // Calcul du score par type
    for (const auto& it : cite->getCarte()) {
        const Hexagone* h = it.second;
        if (!h) continue;

        switch (type) {
        case TypeQuartier::Habitation:
            if (h->getType() == Type::Habitation) {
                score += 1;
            }
            break;

        case TypeQuartier::Marche:
            if (h->getType() == Type::Marche) {
                int ptsAdj = 0;
                try {
                    std::vector<Hexagone*> adjacents = cite->getAdjacents(it.first);
                    for (Hexagone* v : adjacents) {
                        if (!v) continue;
                        if (v->getType() != Type::Marche && v->getType() != Type::Place) {
                            ptsAdj++;
                        }
                    }
                } catch (const CiteException&) {
                    // Si getAdjacents lève une exception, on ignore cet hexagone
                    continue;
                }
                score += ptsAdj;
            }
            break;

        case TypeQuartier::Caserne:
            if (h->getType() == Type::Caserne) {
                bool isIsolee = true;
                try {
                    std::vector<Hexagone*> adjacents = cite->getAdjacents(it.first);
                    for (Hexagone* v : adjacents) {
                        if (!v) continue;
                        if (v->getType() == Type::Caserne) {
                            isIsolee = false;
                            break;
                        }
                    }
                } catch (const CiteException&) {
                    continue;
                }
                if (isIsolee) {
                    score += 1;
                }
            }
            break;

        case TypeQuartier::Temple:
            if (h->getType() == Type::Temple) {
                try {
                    std::vector<Hexagone*> adjacents = cite->getAdjacents(it.first);
                    if (adjacents.size() == 6) {
                        score += 2;
                    }
                } catch (const CiteException&) {
                    continue;
                }
            }
            break;

        case TypeQuartier::Jardin:
            if (h->getType() == Type::Jardin) {
                score += 1;
            }
            break;
        }
    }

    return score * multiplicateur;
}
