#ifndef CHANTIER_H
#define CHANTIER_H

#include <vector>
#include "tuiles.h"
#include "pile.h"
// Dans chantier.h

// ... (début du fichier)

class Chantier {
public:
    Chantier();
    ~Chantier();

    void ajouterPile(Pile& p);

    // Gardez l'implémentation inline ici
    size_t getNbTuiles() const { return tuiles.size(); }

    // CORRECTION 1 : Changer la signature pour correspondre au .cpp
    void retirerTuile(int index); 
    
    // CORRECTION 2 : Ajouter la déclaration manquante
    Tuile* getTuile(int index) const; 

    bool estVide() const;
    void vider();
    void ajouterTuileSpecifique(Tuile* t);

// ... (reste du fichier avec Iterator)
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