#ifndef JEU_CONSOLE_H
#define JEU_CONSOLE_H

#include <vector>
#include <string>
#include <array>
#include "quadrillageView.h"
#include "partie.h" 

class JeuConsole {
public:
    // Point d'entree principal du mode console
    void lancer();

private:
    // Methodes internes d'affichage et de saisie
    void demanderConfiguration();
    void afficherEtatJeu();
    void jouerTour();
    void afficherChantier();

    void nettoyerEcran();
    void afficherRegles();

    // Utilitaires de saisie robustes
    int saisieNombre(const std::string& prompt, int min, int max);
    bool saisieOuiNon(const std::string& prompt);

    // Stockage temporaire des configurations avant l'envoi au moteur
    TuileCite modeTuileCite;
    std::vector<std::string> nomsJoueurs;
    std::array<bool, 5> variantesActives = { false };
    Quadrillage vue;
};

#endif // JEU_CONSOLE_H
