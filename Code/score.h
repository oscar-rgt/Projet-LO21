#ifndef SCORE_H
#define SCORE_H

#include "hexagone.h"

// Structure d'une tuile posée
typedef struct {
    Type type;   // couleur/type
    int niveau;  // hauteur
    int etoiles; // multiplicateur
} ScoreInput;

// voisins[6] contiendra les types des 6 hexagones autour (0 si vide)
int calcul_points(ScoreInput centre, Type voisins[6]);

#endif

