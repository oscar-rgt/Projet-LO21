#include "quadrillageView.h"
#include "except.h"
#include <string>
#include <algorithm>


void Quadrillage::afficher() const { // Affichage console simple, ajout de la description des hexagones hors quadrillage si besoin
    cout << txt << endl;
    if (hors_txt != "") cout << hors_txt << endl;
}


void Quadrillage::remplir(Coord c, const Hexagone* h) {
    // Agrandissement du quadrillage si besoin
    if (c.y >= (maxY)) agrandir('N'); 
    if (c.y <= (minY)) agrandir('S');
    // Ajout d'informations si hexa hors quadrillage
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

    // Calcul du placement graphique du centre de l'hexa
    size_t j = 0;

    int l = c.y * -hauteur_hexa + decalage_ligne; // Ligne du centre de l'hexa
    int col = c.x * largeur_hexa + decalage_col; // Colonne du centre de l'hexa
    j = col + l * longueur_ligne + 1; // Calcul de l'indice du centre de l'hexa 
    if ((c.x % 2)) j += 2 * longueur_ligne; // Décalage si hexa placé en x pair
    if (j > txt.length()) throw AkropolisException("Placement impossible : sortie du quadrillage"); // Exception en cas de valeur d'indice innatendue
    txt.replace(j, 3, h->affiche()); // Modification du quadrillage
}


void Quadrillage::agrandir(char dir) {

    //Agrandissement au sud
    if (dir == 'S') {
        string extension = to_string(minY-1); // Placement du numéro de ligne inférieur
        if (minY > -10) extension += " "; // Décalage si le numéro ne prend que deux caractères (-1 à -9)

        // Première ligne ajoutée
        extension += R"(/     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \  )";
        // Modification de la dernière ligne du quadrillage (Affichage des abscisses) par la première de l'extension
        txt.replace(txt.length() - longueur_ligne, longueur_ligne, extension);

        // Ajout du bas des hexagones d'extension
        txt += R"(
  /       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \ 
  \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       / 
   \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/  
     -9     -8     -7     -6     -5     -4     -3     -2     -1      0      1      2      3      4      5      6      7       8      9    
)";
        minY--; // Réduction du Y minimal car quadrillage agrandi
    }

    // Agrandissement au sud
    if (dir == 'N') {
        // Début de la ligne rajoutée
        string extension = R"(
  /       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \ 
  \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       / 
   \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/       \_____/  
)";
        // Ajout du numéro de ligne supérieur
        extension += to_string(maxY+1);
        // Gestion de la longueur de la ligne avec des espaces en fonctions de la longueur de la chaine de caractères d'affichage du numéro de ligne
        if (maxY < 10) extension += "  ";
        else if (maxY < 100) extension += " ";

        // Ajout de la fin de la ligne numérotée
        extension += R"(/     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \       /     \  )";
        // Ajout de l'extension avant le texte actuel
        txt = extension + txt;
        // Changement de decalage_ligne car la ligne 0 s'est éloignée du haut du quadrillage
        decalage_ligne += 4;
        // Augmentation du Y max suite à l'agrandissement
        maxY++;
    }

}

void Quadrillage::reset() { // Remise du Quadrillage à ses valeurs par défaut
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
    decalage_ligne = 14;
    hauteur_hexa = 4;
    largeur_hexa = 7;
    decalage_col = 68;
    longueur_ligne = 139;
    hors_txt = "";
}