#ifndef JEU_CONSOLE_H
#define JEU_CONSOLE_H

#include <vector>
#include <string>
#include <array>
#include "partie.h" // Lien vers le moteur

class JeuConsole {
public:
    // Point d'entrée principal du mode console
    void lancer();

private:
    // Méthodes internes d'affichage et de saisie
    void demanderConfiguration();
    void afficherEtatJeu();
    void jouerTour();

    // Utilitaires de saisie robustes
    int saisieNombre(const std::string& prompt, int min, int max);
    bool saisieOuiNon(const std::string& prompt);

    // Stockage temporaire des configurations avant l'envoi au moteur
    Partie::TuileCite modeTuileCite;
    std::vector<std::string> nomsJoueurs;
    std::array<bool, 5> variantesActives = { false };
};

#endif // JEU_CONSOLE_H
