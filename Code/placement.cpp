#include <iostream>
#include "placement.h"
#include "tuiles.h"

void Plateau::placer(Tuile* t, Coord c){
	if (!estLibre(c)||!estLibre(c.sudEst())||!estLibre(c.sudOuest())) { return; }
	carte[c] = t->getHexagone(0);
	carte[c.sudEst()] = t->getHexagone(1);
	carte[c.sudOuest()] = t->getHexagone(2);
	
	if(c.z==1){
        std::vector<Coord> voisins = {
                // Voisins de c (qui ne sont pas sudEst ou sudOuest)
                {c.x + 1, c.y, c.z}, {c.x - 1, c.y, c.z},
                {c.x, c.y - 1, c.z},
                {c.x, c.y, c.z + 1}, {c.x, c.y, c.z - 1},
                // Voisins de sudEst (qui ne sont pas c ou sudOuest)
                {c.x - 1, c.y + 1, c.z},
                {c.x, c.y + 2, c.z},
                {c.x, c.y + 1, c.z + 1}, {c.x, c.y + 1, c.z - 1},
                // Voisins de sudOuest (qui ne sont pas c ou sudEst)
                {c.x + 2, c.y + 1, c.z},
                {c.x + 1, c.y + 2, c.z},
                {c.x + 1, c.y + 1, c.z + 1}, {c.x + 1, c.y + 1, c.z - 1}
            };
            
            for (const auto& voisin : voisins) {
                if (!estLibre(voisin)) {
                    touchePlateau = true;
                    break;
                }
            }
            
            if (!touchePlateau ) { 
                printf("Placement impossible")
                return; 
            }
    }
    else {
    // Vérifier que chaque hexagone de la tuile est soutenu par un hexagone en dessous
    Coord dessous_c = {c.x, c.y, c.z - 1};
    Coord dessous_sudEst = {c.x, c.y + 1, c.z - 1};
    Coord dessous_sudOuest = {c.x + 1, c.y + 1, c.z - 1};
    
    if (estLibre(dessous_c) || estLibre(dessous_sudEst) || estLibre(dessous_sudOuest)) {
        printf("Placement impossible ");
        return;
    }
    
    // Vérifier qu'elle est à cheval sur au moins 2 tuiles différentes
    Tuile* tuile_dessous_c = carte[dessous_c].getTuile();
    Tuile* tuile_dessous_sudEst = carte[dessous_sudEst].getTuile();
    Tuile* tuile_dessous_sudOuest = carte[dessous_sudOuest].getTuile();
    
    bool deuxTuilesDifferentes = (tuile_dessous_c != tuile_dessous_sudEst) || 
                                  (tuile_dessous_c != tuile_dessous_sudOuest) || 
                                  (tuile_dessous_sudEst != tuile_dessous_sudOuest);
    
    if (!deuxTuilesDifferentes) {
        printf("Placement impossible ");
        return;
    }
}
	
}
