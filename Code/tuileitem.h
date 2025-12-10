#ifndef TUILEITEM_H
#define TUILEITEM_H

#include <QGraphicsItemGroup>
#include "tuiles.h"
#include "hexagoneitem.h"

class TuileItem : public QGraphicsItemGroup {
public:
    // On garde un pointeur vers le modèle pour récupérer l'ID ou le prix au clic
    Tuile* modeleTuile;
    int indexChantier; // Pour savoir quelle tuile c'est dans le vecteur du chantier

    TuileItem(Tuile* t, int index, double rayon = 30.0);

    // Helpers pour récupérer la couleur (à déplacer idéalement dans un utilitaire partagé)
    QColor getTypeColor(TypeQuartier t);
};

#endif // TUILEITEM_H
