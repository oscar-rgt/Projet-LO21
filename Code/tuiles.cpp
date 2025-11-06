#include <iostream>
#include <cstring>
#include "tuiles.h"
#include "hexagone.h"

using namespace std;

Tuile::Tuile(int i, Hexagone& h1, Hexagone& h2, Hexagone& h3) : id(i), orientation(N), hexagones{ h1, h2, h3 } {}








