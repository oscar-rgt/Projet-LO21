#ifndef TUILEITEM_H
#define TUILEITEM_H

#include <QGraphicsItemGroup>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include "tuiles.h"
#include "hexagoneitem.h"

class TuileItem : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    Tuile* modeleTuile;  // Pointeur vers le modèle de la tuile
    int indexChantier;    // Index de la tuile dans le chantier

    // Constructeur : prend une tuile, son index dans le chantier, et le rayon des hexagones
    TuileItem(Tuile* t, int index, double rayon = 30.0);

    // Méthode pour obtenir la couleur associée à un type de quartier
    static QColor getTypeColor(TypeQuartier t);

    void setSelection(bool estSelectionne);

protected:
    // Gestion de l'événement de clic sur la tuile
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    // Signal émis quand la tuile est cliquée (avec son index dans le chantier)
    void clicked(int index);
};

#endif // TUILEITEM_H
