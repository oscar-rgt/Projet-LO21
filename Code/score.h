#ifndef SCORE_H
#define SCORE_H

#include "hexagone.h"
#include <vector>

using namespace std;

int calculPoints(Hexagone& h, vector<vector<Hexagone>>& plateau, int ligne, int colonne);
int calculScore(vector<vector<Hexagone>>& plateau);

#endif
