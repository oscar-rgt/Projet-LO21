#ifndef JEU_CONSOLE_H
#define JEU_CONSOLE_H

#include <vector>
#include <string>
#include <array>
#include "quadrillageView.h"
#include "partie.h" 

using namespace std;

class JeuConsole {
public:
    void lancer();

private:
    void demanderConfiguration();
    void afficherEtatJeu();
    void jouerTour();
    void afficherChantier();
    void nettoyerEcran();
    void afficherRegles();

    int saisieNombre(const string& prompt, int min, int max);
    bool saisieOuiNon(const string& prompt);

    TuileCite modeTuileCite;
    vector<string> nomsJoueurs;
    array<bool, 5> variantesActives = { false };
    Quadrillage vue;
};

#endif 
