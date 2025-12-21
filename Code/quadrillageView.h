#include <string>
#include "tuiles.h"
#include "coord.h"

// Classe permettant l'affichage du quadrillage dans la console
class Quadrillage {
private:
    string txt; // Texte du quadrillage
/// VARIABLES PERMETTANT LE CALCUL DE L'EMPLACEMENT DU CENTRE D'UN HEXA
    int  maxX, minX , maxY , minY; // Limites du quadrillage pour gérer son agrandissement
    int decalage_ligne; // Décalage en lignes entre le haut du quadrillage et la ligne 0 (hexa 0,0)
    int hauteur_hexa; // Hauteur d'un hexa
    int largeur_hexa; // Largeur d'un hexa
    int decalage_col; // Décalage en colonnes entre la gauche du quadrillage et la colonne 0
    int longueur_ligne;
///////////////////////////////////////////////////////////////////////
    string hors_txt; // Texte pour afficher les emplacement des hexagones hors de l'affichage graphique
public:
    Quadrillage() = default; 
    void remplir(Coord c, const Hexagone* h); // Placement d'un hexagone h aux Coord c dans l'affichage (non const car modification de txt)
    void reset(); // Remise à 0 du quadrillage (non const car modification de txt)
    void agrandir(char dir); // Agrandir le quadrillage dans une direction (ici, N ou S) (non const car modification de txt)
    void afficher() const; // Affichage du quadrillage dans la console 
};