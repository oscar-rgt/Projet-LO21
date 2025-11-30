#ifndef IA_H
#define IA_H

#include "joueur.h"
#include "chantier.h"
#include <vector>

class IA : public Joueur {
private:
    int difficulte; // 1: Hippodamos, 2: Métagénès, 3: Callicratès
    std::vector<Tuile*> tuilesAcquises;

public:
    IA(const string& nom, int diff) : Joueur(nom), difficulte(diff) {}
    virtual ~IA();

    // Méthode pour choisir une tuile dans le chantier
    // Retourne l'index de la tuile choisie
    int choisirTuile(const Chantier& chantier);

    // Ajoute la tuile aux tuiles acquises (sans la placer dans une cité)
    void ajouterTuile(Tuile* t);

    // Calcule le score selon la difficulté
    int calculerScoreIA() const;

    // Helper pour vérifier si une tuile contient une Place
    static bool tuileContientPlace(Tuile* t);
};

#endif
