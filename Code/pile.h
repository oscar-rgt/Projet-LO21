#ifndef PILE_H
#define PILE_H

class Pile {
public:
    int id;         // Identifiant de la pile (facultatif)
    int nbTuiles;   // Nombre de tuiles restantes dans la pile
    Pile(int id_, int nb);
    bool retirerTuile();
};

#endif
