#ifndef CHANTIER_H
#define CHANTIER_H

#include <vector>
#include "tuiles.h"
#include "pile.h"

class Chantier {
public:
    Chantier();
    ~Chantier();

    // Gestion des tuiles
    void ajouterPile(Pile& p);

    size_t getNbTuiles() const { return tuiles.size(); }

    void retirerTuile(Tuile* t); // Modifié pour prendre un pointeur
    bool estVide() const;

    // Nettoyage
    void vider();

    // Accesseurs pour l'affichage (références constantes)
    // SUPPRIMÉ : const std::vector<Tuile*>& getTuiles() const { return tuiles; }
    void ajouterTuileSpecifique(Tuile* t);

    // ==========================================
    // ITERATOR
    // ==========================================
    class Iterator {
        friend class Chantier;
        std::vector<Tuile*>::const_iterator current;
        Iterator(std::vector<Tuile*>::const_iterator it) : current(it) {}
    public:
        Iterator() {}
        Iterator& operator++() { ++current; return *this; }
        bool operator!=(const Iterator& other) const { return current != other.current; }
        bool operator==(const Iterator& other) const { return current == other.current; }
        Tuile* operator*() const { return *current; }
    };

    Iterator begin() const { return Iterator(tuiles.begin()); }
    Iterator end() const { return Iterator(tuiles.end()); }

private:
    std::vector<Tuile*> tuiles;
};

#endif