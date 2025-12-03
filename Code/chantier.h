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

    Tuile* getTuile(int index) const;
    void retirerTuile(int index);
    size_t getNbTuiles() const;
    bool estVide() const;


    // Nettoyage
    void vider();

    // Accesseurs pour l'affichage (références constantes)
    const std::vector<Tuile*>& getTuiles() const { return tuiles; }
    void ajouterTuileSpecifique(Tuile* t);


private:
    std::vector<Tuile*> tuiles;

};

#endif
