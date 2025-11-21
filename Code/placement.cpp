#include <iostream>
#include "placement.h"
#include "tuiles.h"
using namespace std;

const bool Cite::toucheCite(Coord c) const { 
    """L'hexagone aux coos c touche-t-il la cité ?""";
    return (!estLibre({ c.x - 1, c.y - 1, c.z }) || !estLibre({ c.x, c.y - 1, c.z }) || !estLibre({ c.x + 1, c.y - 1, c.z })
        || !estLibre({ c.x - 1, c.y, c.z }) || !estLibre({ c.x, c.y + 1, c.z }) || !estLibre({ c.x + 1, c.y, c.z }));
}

void Cite::placer(Tuile* t, Coord c){
    Coord c2 = c.sudEst(), c3 = c.sudOuest();
    if (!estLibre(c) || !estLibre(c2) || !estLibre(c3))
        throw CiteException("Placement impossible : Une des cases n'est pas libre.");

	
    if (c.z == 0) if (!toucheCite(c) && !toucheCite(c2) && !toucheCite(c3)) throw CiteException("Placement impossible : L'emplacement ne touche pas la cité.");
    else {
    // Vérifier que chaque hexagone de la tuile est soutenu par un hexagone en dessous
    Coord dessous_c = {c.x, c.y, c.z - 1};
    Coord dessous_sudEst = {c2.x, c2.y, c2.z - 1};
    Coord dessous_sudOuest = {c3.x, c3.y, c3.z - 1};
    
    if (estLibre(dessous_c) || estLibre(dessous_sudEst) || estLibre(dessous_sudOuest)) throw CiteException("Placement impossible : Les cases inférieures sont vides.");
    // A revoir : un hexagone ne sait pas à quelle tuile il appartient => Pas de getTuile
    ////////////////////////////////////////////////////////////////////////////////
    // Vérifier qu'elle est à cheval sur au moins 2 tuiles différentes
    //Tuile* tuile_dessous_c = carte[dessous_c].getTuile();
    //Tuile* tuile_dessous_sudEst = carte[dessous_sudEst].getTuile();
    //Tuile* tuile_dessous_sudOuest = carte[dessous_sudOuest].getTuile();
    ////////////////////////////////////////////////////////////////////////////////
    


    //Plus rien ne fonctionne ici dcp
    /*
    bool deuxTuilesDifferentes = (tuile_dessous_c != tuile_dessous_sudEst) || 
                                  (tuile_dessous_c != tuile_dessous_sudOuest) || 
                                  (tuile_dessous_sudEst != tuile_dessous_sudOuest);
    
    if (!deuxTuilesDifferentes) 
        throw CiteException("...");
    }*/
    carte[c] = &(t->getHexagone(0));
    carte[c2] = &(t->getHexagone(1));
    carte[c3] = &(t->getHexagone(2));
}
	
}



