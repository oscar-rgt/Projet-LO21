#ifndef TUILEITEM_H
#define TUILEITEM_H

#include <QGraphicsItemGroup>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include "tuiles.h"
#include "hexagoneitem.h"

//hérite de 2 classes de qt :
//QObject : pour la communication (signaux, slots, animations)
//QGraphicsItemGroup : pour l'affichage graphique groupé
class TuileItem : public QObject, public QGraphicsItemGroup
{
    //macros indispensables pour les héritages qt
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    Tuile* modeleTuile;  
    int indexChantier;    

    
    TuileItem(Tuile* t, int index, double rayon = 30.0);

    static QColor getTypeColor(TypeQuartier t);

    void setSelection(bool estSelectionne);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void clicked(int index);
};

#endif 
