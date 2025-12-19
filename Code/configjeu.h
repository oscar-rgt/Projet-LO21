#ifndef CONFIG_JEU_H
#define CONFIG_JEU_H

#include <array>
#include <vector>
#include <string>

// Enum déplacé ici pour être accessible partout
enum class TuileCite { STANDARD, AUGMENTE };

struct ConfigJeu {
    int nbJoueurs;
    TuileCite modeTuileCite;
    std::array<bool, 5> variantes; // Habitation, Marché, Caserne, Temple, Jardin
    unsigned int niveauIllustreConstructeur; // 0 = Pas d'IA ou Multi

    ConfigJeu() : nbJoueurs(2), modeTuileCite(TuileCite::STANDARD), variantes({false}), niveauIllustreConstructeur(0) {}

    // Helper pour savoir si on est en solo
    bool estModeSolo() const { return niveauIllustreConstructeur > 0; }
};

#endif // CONFIG_JEU_H
