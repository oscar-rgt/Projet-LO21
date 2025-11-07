#include "score.h"
#include <iostream>

//constructeur
Score::Score(Joueur* j, Cité* c)
    : joueur(j), cite(c), total(0)
{
    for (auto type : {TypeQuartier::Habitation, TypeQuartier::Marche,
                      TypeQuartier::Caserne, TypeQuartier::Temple,
                      TypeQuartier::Jardin}) {
        pointsParType[type] = 0;
    }
}

// Getters
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

//score globale
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

//score par type
int Score::calculerScoreType(TypeQuartier type) {
    int score = 0;
    int multiplicateur = 0;

    // Récupère toutes les cases de la cité
    const std::vector<Hexagone*>& hexas = cite->getHexagones();

    // 1️⃣ Calcul du multiplicateur total pour ce type (étoiles)
    for (auto* h : hexas) {
        if (h->getType() == Type::Place) {
            // On ajoute les étoiles de la couleur correspondante
            // On suppose que getEtoiles() donne bien le nombre d’étoiles de cette place
            // et qu’elle est associée à un type de quartier
            // (ce lien doit être stocké ailleurs : à adapter selon ton implémentation)
            // Ici, on suppose que chaque place a un "type associé" accessible via une méthode
            // ex: h->getTypeAssocie()
            if (h->getTypeAssocie() == type) {
                multiplicateur += h->getEtoiles();
            }
        }
    }

    // Si aucune étoile de ce type, score = 0 directement
    if (multiplicateur == 0)
        return 0;

    // 2️⃣ Calcul des points bruts selon le type de quartier
    switch (type) {
        // -------------------------------------
        case TypeQuartier::Habitation: {
            // Trouver les groupes contigus d’habitations
            // On utilise une exploration BFS/DFS pour repérer les groupes
            std::vector<Hexagone*> visites;
            int maxGroupe = 0;

            for (auto* h : hexas) {
                if (h->getType() == Type::Habitation &&
                    std::find(visites.begin(), visites.end(), h) == visites.end()) {

                    // Nouveau groupe
                    std::vector<Hexagone*> pile = {h};
                    int tailleGroupe = 0;

                    while (!pile.empty()) {
                        Hexagone* actuel = pile.back();
                        pile.pop_back();

                        if (std::find(visites.begin(), visites.end(), actuel) != visites.end())
                            continue;

                        visites.push_back(actuel);
                        tailleGroupe++;

                        for (auto* voisin : cite->getAdjacents(actuel)) {
                            if (voisin->getType() == Type::Habitation &&
                                std::find(visites.begin(), visites.end(), voisin) == visites.end()) {
                                pile.push_back(voisin);
                            }
                        }
                    }
                    if (tailleGroupe > maxGroupe) maxGroupe = tailleGroupe;
                }
            }

            score = maxGroupe * multiplicateur;
            break;
        }

        // -------------------------------------
        case TypeQuartier::Marche: {
            for (auto* h : hexas) {
                if (h->getType() == Type::Marche) {
                    std::vector<Type> typesAdjacents;
                    for (auto* v : cite->getAdjacents(h)) {
                        if (v->getType() != Type::Marche &&
                            v->getType() != Type::Place) {
                            if (std::find(typesAdjacents.begin(), typesAdjacents.end(), v->getType()) == typesAdjacents.end()) {
                                typesAdjacents.push_back(v->getType());
                            }
                        }
                    }
                    score += (int)typesAdjacents.size();
                }
            }
            score *= multiplicateur;
            break;
        }

        // -------------------------------------
        case TypeQuartier::Caserne: {
            for (auto* h : hexas) {
                if (h->getType() == Type::Caserne) {
                    bool isIsolated = true;
                    for (auto* v : cite->getAdjacents(h)) {
                        if (v->getType() == Type::Caserne) {
                            isIsolated = false;
                            break;
                        }
                    }
                    if (isIsolated) score += 1;
                }
            }
            score *= multiplicateur;
            break;
        }

        // -------------------------------------
        case TypeQuartier::Temple: {
            for (auto* h : hexas) {
                if (h->getType() == Type::Temple) {
                    const auto& adj = cite->getAdjacents(h);
                    if (adj.size() == 6) {
                        score += 2;
                    }
                }
            }
            score *= multiplicateur;
            break;
        }

        // -------------------------------------
        case TypeQuartier::Jardin: {
            for (auto* h : hexas) {
                if (h->getType() == Type::Jardin) {
                    score += 1;
                }
            }
            score *= multiplicateur;
            break;
        }
    }

    return score;
}
