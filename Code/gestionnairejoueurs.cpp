#include "gestionnairejoueurs.h"
#include "except.h"
#include "score.h"
#include <algorithm>
#include <random>
#include <map>

GestionnaireJoueurs::GestionnaireJoueurs() : indexJoueurActuel(0) {}

GestionnaireJoueurs::~GestionnaireJoueurs() {
    vider();
}

void GestionnaireJoueurs::vider() {
    for (auto j : joueurs) delete j;
    joueurs.clear();
    indexJoueurActuel = 0;
}

void GestionnaireJoueurs::initialiser(const std::vector<std::string>& noms, bool modeSolo, unsigned int niveauIA) {
    vider();
    for (const auto& nom : noms) {
        joueurs.push_back(new Joueur(nom));
    }

    if (modeSolo) {
        if (!joueurs.empty()) joueurs[0]->utiliserPierres(1); //en solo, l'humain a 1 pierre au début
        joueurs.push_back(new IA("Illustre Architecte", niveauIA));
    }
}

void GestionnaireJoueurs::passerAuSuivant() {
    if (joueurs.empty()) return;
    indexJoueurActuel = (indexJoueurActuel + 1) % joueurs.size();
}

Joueur* GestionnaireJoueurs::getJoueurActuel() const {
    if (joueurs.empty()) throw AkropolisException("Aucun joueur dans la partie.");
    return joueurs[indexJoueurActuel];
}



void GestionnaireJoueurs::designerArchitecteChefAleatoire() {
    if (joueurs.empty()) return;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(joueurs.begin(), joueurs.end(), gen);
    indexJoueurActuel = 0;
}

void GestionnaireJoueurs::ajouterJoueurExistant(Joueur* j) {
    joueurs.push_back(j);
}

std::vector<Joueur*> GestionnaireJoueurs::determinerGagnants() {
    int maxScore = -1;
    std::map<Joueur*, int> memoScores;

    //calculer tous les scores
    for (auto j : joueurs) {
        int s = j->getScore()->calculerScore();
        memoScores[j] = s;
        if (s > maxScore) maxScore = s;
    }

    //trouver les egalités
    std::vector<Joueur*> candidats;
    for (auto j : joueurs) {
        if (memoScores[j] == maxScore) candidats.push_back(j);
    }

    //on départage aux pierres
    if (candidats.size() > 1) {
        int maxPierres = -1;
        for (auto j : candidats) {
            if (j->getPierres() > maxPierres) maxPierres = j->getPierres();
        }
        std::vector<Joueur*> gagnantsFinaux;
        for (auto j : candidats) {
            if (j->getPierres() == maxPierres) gagnantsFinaux.push_back(j);
        }
        return gagnantsFinaux;
    }

    return candidats;
}
