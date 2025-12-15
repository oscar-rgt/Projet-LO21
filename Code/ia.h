#ifndef IA_H
#define IA_H

#include "joueur.h"
#include "chantier.h"
#include <vector>

class IA : public Joueur {
private:
    int difficulte; // 1: Hippodamos, 2: Métagénès, 3: Callicratès
    std::vector<Tuile*> tuilesAcquises;
	TuileDepart* tuileDepart; // Tuile de départ de l'IA

public:
    IA(const string& nom, int diff);
    virtual ~IA();

    // Méthode pour choisir une tuile dans le chantier
    // Retourne l'index de la tuile choisie
    int choisirTuile(const Chantier& chantier);

    // Ajoute la tuile aux tuiles acquises (sans la placer dans une cité)
    void ajouterTuile(Tuile* t);

    // Calcule le score selon la difficulté
    int calculerScoreIA() const;

    // ==========================================
    // ITERATEUR POUR LES TUILES ACQUISES
    // ==========================================
    class Iterator {
        friend class IA;
        std::vector<Tuile*>::const_iterator current;
        Iterator(std::vector<Tuile*>::const_iterator it) : current(it) {}
    public:
        Iterator() {}
        Iterator& operator++() { ++current; return *this; }
        bool operator!=(const Iterator& other) const { return current != other.current; }
        bool operator==(const Iterator& other) const { return current == other.current; }
        Tuile* operator*() const { return *current; }
    };

    // Méthodes pour récupérer l'itérateur
    Iterator begin() const { return Iterator(tuilesAcquises.begin()); }
    Iterator end() const { return Iterator(tuilesAcquises.end()); }


    // Helper pour vérifier si une tuile contient une Place
    static bool tuileContientPlace(Tuile* t);

    // Helper pratique pour savoir combien de tuiles afficher (QT)
    int getNbTuilesAcquises() const { return static_cast<int>(tuilesAcquises.size()); }
};

#endif
