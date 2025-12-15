#include <string>
#include "tuiles.h"
#include "coord.h"
class Quadrillage {
private:
    string txt;
    int  maxX, minX , maxY , minY;
    int line_offset;
    int hex_height;
    int hex_width;
    int col_offset;
    int line_length;
    string hors_txt;
public:
    Quadrillage() = default;
    void remplir(Coord c, const Hexagone* h);
    void reset();
    void agrandir(char dir);
    void afficher() const;
};