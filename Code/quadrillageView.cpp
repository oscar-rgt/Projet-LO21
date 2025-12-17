#include "quadrillageView.h"
#include "except.h"
#include <string>
#include <algorithm>


void Quadrillage::afficher() const {
    cout << txt << endl;
    if (hors_txt != "") cout << hors_txt << endl;
}


void Quadrillage::remplir(Coord c, const Hexagone* h) {
    for (int i = 0; i < 3; i++) {
        if (c.y >= (maxY)) agrandir('N');
        if (c.y <= (minY)) agrandir('S');
        if (c.x > (maxX) || c.x < (minX)) {
        hors_txt += " [";
        hors_txt += to_string(c.x);
        hors_txt += ", ";
        hors_txt += to_string(c.y);
        hors_txt += ", ";
        hors_txt += to_string(c.z);
        hors_txt += "] : ";
        hors_txt += h->affiche();
        hors_txt += "\n";
        }
        size_t j = 0;

        int l = c.y * -hex_height + line_offset;
        int col = c.x * hex_width + col_offset;
        j = col + l * line_length + 1;
        if ((c.x % 2)) j += 2 * line_length;
        if (j > txt.length()) throw AkropolisException("Placement impossible : sortie du quadrillage");
        //replace(txt.begin(), txt.end(), ' ', '.'); //debug ascii
        txt.replace(j, 3, h->affiche());
    }
}


void Quadrillage::agrandir(char dir) {
    if (dir == 'S') {
        minY--;
        string extension = to_string(minY);
        if (minY > -10) extension += " ";
        extension += R"(/     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \  )";
        txt.replace(txt.length() - line_length, line_length, extension);
        txt += R"(
  /       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \ 
  \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       / 
   \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/  
     -9     -8     -7     -6     -5     -4     -3     -2     -1      0      1      2      3      4      5      6      7       8      9    
)";
    }
    if (dir == 'N') {
        maxY++;
        string extension = R"(
  /       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \ 
  \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       / 
   \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/  
)";
        extension += to_string(maxY);
        if (maxY < 10) extension += "  ";
        else if (maxY < 100) extension += " ";
        extension += R"(/     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \  )";
        txt = extension + txt;
        line_offset += 4;
    }

}

void Quadrillage::reset() {
    txt=R"(
  /       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \ 
  \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       / 
   \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/  
3  /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \  
  /       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \ 
  \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       / 
   \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/  
2  /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \  
  /       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \ 
  \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       / 
   \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/  
1  /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \  
  /       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \ 
  \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       / 
   \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/  
0  /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \  
  /       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \ 
  \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       / 
   \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/  
-1 /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \  
  /       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \ 
  \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       / 
   \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/  
-2 /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \  
  /       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \ 
  \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       / 
   \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/  
     -9     -8     -7     -6     -5     -4     -3     -2     -1      0      1      2      3      4      5      6      7      8      9     
)";
    maxX = 9;
    minX = -9;
    maxY = 3;
    minY = -2;
    line_offset = 14;
    hex_height = 4;
    hex_width = 7;
    col_offset = 68;
    line_length = 139;
    hors_txt = "";
}