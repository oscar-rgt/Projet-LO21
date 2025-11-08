#ifndef PILE_H
#define PILE_H

class Pile {
public:
    int id;  
    int nbTuiles;
    Pile(int id_, int nb);
    bool retirerTuile();
};

#endif
