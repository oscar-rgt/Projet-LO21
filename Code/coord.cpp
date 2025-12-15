#include "coord.h"

// Mise à jour selon votre nouvelle géométrie "Index 2"
Coord Coord::cote(bool inversion) const {
    if (inversion) return { x + 1, x % 2 == 0 ? y : y - 1, z };
    return { x - 1, x % 2 == 0 ? y : y - 1, z };
}
